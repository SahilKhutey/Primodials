/*
# Create simulation_snapshots table (single-tenant, no auth)

1. New Tables
- `simulation_snapshots`
  - `id` (uuid, primary key)
  - `name` (text, not null) — user-given label for the saved experiment
  - `tick` (integer, not null) — simulation tick at capture time
  - `population` (integer, not null) — organism count at capture
  - `species_count` (integer, not null) — distinct species count at capture
  - `generations` (integer, not null) — max generations reached
  - `snapshot` (jsonb, not null) — full serializable simulation state (organisms, food, species, stats, settings)
  - `created_at` (timestamptz, default now)
2. Security
- Enable RLS on `simulation_snapshots`.
- Allow anon + authenticated full CRUD because the data is intentionally shared/public (no sign-in screen).
3. Indexes
- Index on `created_at` descending for history listing.
*/

CREATE TABLE IF NOT EXISTS simulation_snapshots (
  id uuid PRIMARY KEY DEFAULT gen_random_uuid(),
  name text NOT NULL,
  tick integer NOT NULL,
  population integer NOT NULL,
  species_count integer NOT NULL,
  generations integer NOT NULL,
  snapshot jsonb NOT NULL,
  created_at timestamptz DEFAULT now()
);

CREATE INDEX IF NOT EXISTS idx_simulation_snapshots_created_at
  ON simulation_snapshots (created_at DESC);

ALTER TABLE simulation_snapshots ENABLE ROW LEVEL SECURITY;

DROP POLICY IF EXISTS "anon_select_snapshots" ON simulation_snapshots;
CREATE POLICY "anon_select_snapshots" ON simulation_snapshots FOR SELECT
  TO anon, authenticated USING (true);

DROP POLICY IF EXISTS "anon_insert_snapshots" ON simulation_snapshots;
CREATE POLICY "anon_insert_snapshots" ON simulation_snapshots FOR INSERT
  TO anon, authenticated WITH CHECK (true);

DROP POLICY IF EXISTS "anon_update_snapshots" ON simulation_snapshots;
CREATE POLICY "anon_update_snapshots" ON simulation_snapshots FOR UPDATE
  TO anon, authenticated USING (true) WITH CHECK (true);

DROP POLICY IF EXISTS "anon_delete_snapshots" ON simulation_snapshots;
CREATE POLICY "anon_delete_snapshots" ON simulation_snapshots FOR DELETE
  TO anon, authenticated USING (true);
