#include "Simulation/Snapshots/SimulationSnapshot.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Simulation/Components/LineageComponents.hpp"
#include "Simulation/Components/ResourceComponents.hpp"
#include "Simulation/SimulationState.hpp"
#include "Core/Logger.hpp"
#include <fstream>
#include <vector>
#include <cstring>

namespace Shape::Simulation {

// A simple binary serialization format for Phase D.
// It manually serializes known components for living entities.

struct SnapshotHeader {
    char magic[4] = {'S','H','S','N'};
    u32  version  = 1;
    u64  tick     = 0;
    u32  entity_count = 0;
};

// Bitmask of which components an entity has
enum ComponentMask : u32 {
    HAS_POSITION         = 1 << 0,
    HAS_VELOCITY         = 1 << 1,
    HAS_CREATURE_STATE   = 1 << 2,
    HAS_CREATURE_GENOME  = 1 << 3,
    HAS_DERIVED_ATTR     = 1 << 4,
    HAS_COMBAT_STATS     = 1 << 5,
    HAS_COMBAT_TRACKER   = 1 << 6,
    HAS_MATING_STATUS    = 1 << 7,
    HAS_ANCESTORS        = 1 << 8,
    HAS_COLOR_OVERRIDE   = 1 << 9,
    HAS_FOOD_DEPOSIT     = 1 << 10,
};

static void write_bytes(std::vector<u8>& buf, const void* data, usize size) {
    const u8* ptr = static_cast<const u8*>(data);
    buf.insert(buf.end(), ptr, ptr + size);
}

template<typename T>
static void write_comp(std::vector<u8>& buf, const T& comp) {
    write_bytes(buf, &comp, sizeof(T));
}

static void read_bytes(const u8*& ptr, void* data, usize size) {
    std::memcpy(data, ptr, size);
    ptr += size;
}

template<typename T>
static void read_comp(const u8*& ptr, T& comp) {
    read_bytes(ptr, &comp, sizeof(T));
}

std::vector<u8> SimulationSnapshot::save_to_memory(const ECS2::World2& world) {
    std::vector<u8> buf;
    buf.reserve(1024 * 1024); // 1 MB reserve

    SnapshotHeader header;
    header.tick = world.resource<WorldState>().environment.current_tick;
    // We'll rewrite entity_count later
    const usize header_offset = buf.size();
    write_comp(buf, header);

    // Save world state
    write_comp(buf, world.resource<WorldState>());

    u32 entity_count = 0;
    
    // We serialize all entities with Position
    // We can't iterate ALL entities directly easily without a query, 
    // but almost all entities have Position.
    const_cast<ECS2::World2&>(world).for_each<ECS2::EntityId, Position>([&](ECS2::EntityId e, const Position& pos) {
        entity_count++;
        
        write_comp(buf, e);
        
        u32 mask = HAS_POSITION;
        if (world.has<Velocity>(e))          mask |= HAS_VELOCITY;
        if (world.has<CreatureState>(e))     mask |= HAS_CREATURE_STATE;
        if (world.has<CreatureGenome>(e))    mask |= HAS_CREATURE_GENOME;
        if (world.has<DerivedAttributes>(e)) mask |= HAS_DERIVED_ATTR;
        if (world.has<CombatStats>(e))       mask |= HAS_COMBAT_STATS;
        if (world.has<CombatTracker>(e))     mask |= HAS_COMBAT_TRACKER;
        if (world.has<MatingStatus>(e))      mask |= HAS_MATING_STATUS;
        if (world.has<Ancestors>(e))         mask |= HAS_ANCESTORS;
        if (world.has<ColorOverride>(e))     mask |= HAS_COLOR_OVERRIDE;
        if (world.has<FoodDeposit>(e))       mask |= HAS_FOOD_DEPOSIT;

        write_comp(buf, mask);

        write_comp(buf, pos);
        if (mask & HAS_VELOCITY)         write_comp(buf, world.get<Velocity>(e));
        if (mask & HAS_CREATURE_STATE)   write_comp(buf, world.get<CreatureState>(e));
        if (mask & HAS_CREATURE_GENOME)  write_comp(buf, world.get<CreatureGenome>(e));
        if (mask & HAS_DERIVED_ATTR)     write_comp(buf, world.get<DerivedAttributes>(e));
        if (mask & HAS_COMBAT_STATS)     write_comp(buf, world.get<CombatStats>(e));
        if (mask & HAS_COMBAT_TRACKER)   write_comp(buf, world.get<CombatTracker>(e));
        if (mask & HAS_MATING_STATUS)    write_comp(buf, world.get<MatingStatus>(e));
        if (mask & HAS_ANCESTORS)        write_comp(buf, world.get<Ancestors>(e));
        if (mask & HAS_COLOR_OVERRIDE)   write_comp(buf, world.get<ColorOverride>(e));
        if (mask & HAS_FOOD_DEPOSIT)     write_comp(buf, world.get<FoodDeposit>(e));
    });

    // Patch header with entity count
    header.entity_count = entity_count;
    std::memcpy(buf.data() + header_offset, &header, sizeof(header));

    return buf;
}

bool SimulationSnapshot::load_from_memory(ECS2::World2& world, const std::vector<u8>& blob) {
    if (blob.size() < sizeof(SnapshotHeader)) return false;
    
    const u8* ptr = blob.data();
    SnapshotHeader header;
    read_comp(ptr, header);

    if (header.magic[0] != 'S' || header.magic[1] != 'H' ||
        header.magic[2] != 'S' || header.magic[3] != 'N') {
        SHAPE_LOG_ERROR("Snapshot", "Invalid magic bytes in snapshot");
        return false;
    }
    
    if (header.version != 1) {
        SHAPE_LOG_ERROR("Snapshot", "Unsupported snapshot version {}", header.version);
        return false;
    }

    WorldState ws;
    read_comp(ptr, ws);
    world.set_resource(ws);

    // We do NOT clear the world pool here because World2 doesn't have an easy reset.
    // In a real load we'd need to destroy all existing entities first.
    // For Phase D time scrubbing, we assume the world is either fresh or we
    // handle it externally. But let's at least destroy existing entities that have Position.
    std::vector<ECS2::EntityId> to_destroy;
    world.for_each<ECS2::EntityId, Position>([&](ECS2::EntityId e, Position&) {
        to_destroy.push_back(e);
    });
    for (auto e : to_destroy) {
        world.destroy(e);
    }

    // Now load entities
    for (u32 i = 0; i < header.entity_count; ++i) {
        ECS2::EntityId e;
        read_comp(ptr, e);

        // We create a new entity. It might not match the saved EntityId exactly
        // if the pool generations have advanced, but it will have the same index
        // in our simple pool (since we just emptied it).
        ECS2::EntityId new_e = world.create();

        u32 mask = 0;
        read_comp(ptr, mask);

        if (mask & HAS_POSITION) {
            Position comp; read_comp(ptr, comp); world.add(new_e, comp);
            // Also need SpatialTag
            world.add(new_e, SpatialTag{});
        }
        if (mask & HAS_VELOCITY) {
            Velocity comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
        if (mask & HAS_CREATURE_STATE) {
            CreatureState comp; read_comp(ptr, comp); world.add(new_e, comp);
            world.add(new_e, AIState{});
            world.add(new_e, FitnessScore{});
            world.add(new_e, ReproductionState{});
        }
        if (mask & HAS_CREATURE_GENOME) {
            CreatureGenome comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
        if (mask & HAS_DERIVED_ATTR) {
            DerivedAttributes comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
        if (mask & HAS_COMBAT_STATS) {
            CombatStats comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
        if (mask & HAS_COMBAT_TRACKER) {
            CombatTracker comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
        if (mask & HAS_MATING_STATUS) {
            MatingStatus comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
        if (mask & HAS_ANCESTORS) {
            Ancestors comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
        if (mask & HAS_COLOR_OVERRIDE) {
            ColorOverride comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
        if (mask & HAS_FOOD_DEPOSIT) {
            FoodDeposit comp; read_comp(ptr, comp); world.add(new_e, comp);
        }
    }

    SHAPE_LOG_INFO("Snapshot", "Loaded {} entities at tick {}", header.entity_count, header.tick);
    return true;
}

bool SimulationSnapshot::save_to_file(const ECS2::World2& world, const std::string& path) {
    std::vector<u8> blob = save_to_memory(world);
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    out.write(reinterpret_cast<const char*>(blob.data()), blob.size());
    return true;
}

bool SimulationSnapshot::load_from_file(ECS2::World2& world, const std::string& path) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    if (!in) return false;
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<u8> blob(size);
    if (!in.read(reinterpret_cast<char*>(blob.data()), size)) return false;
    return load_from_memory(world, blob);
}

} // namespace Shape::Simulation
