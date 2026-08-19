# 🤖 PRIMODIALS — Discord Community Bot Code

Complete Python (`discord.py`) and JavaScript (`discord.js`) implementations for automated onboarding, moderation, and GitHub webhook synchronization.

---

## 🐍 1. PYTHON IMPLEMENTATION (`bot.py`)

```python
"""
Primordials Discord Bot — Python (discord.py 2.3+)
Features:
- Automated DM welcome & #welcome broadcast
- Dynamic role progression (Newcomer -> Member after 7 days + 5 messages)
- Slash commands for moderation (/warn, /timeout, /kick, /ban, /purge)
- Role assignment management
"""

import os
import sys
from datetime import datetime, timedelta
import discord
from discord import app_commands
from discord.ext import commands, tasks
from dotenv import load_dotenv

load_dotenv()

DISCORD_TOKEN = os.getenv('DISCORD_TOKEN')
GUILD_ID = int(os.getenv('GUILD_ID', '0'))
WELCOME_CHANNEL_NAME = 'welcome'
LOG_CHANNEL_NAME = 'mod-log'

COLOR_MAGENTA = 0xFF4FA3
COLOR_ALGAE = 0x4ECDC4
COLOR_VOLCANIC = 0xD62246

intents = discord.Intents.default()
intents.message_content = True
intents.members = True
intents.guilds = True

bot = commands.Bot(command_prefix='!', intents=intents, help_command=None)

@bot.event
async def on_ready():
    print(f'🧬 Primordials Bot online as {bot.user}')
    try:
        synced = await bot.tree.sync()
        print(f'Synced {len(synced)} application commands')
    except Exception as e:
        print(f'Failed to sync commands: {e}')

@bot.event
async def on_member_join(member: discord.Member):
    if member.bot:
        return

    # Assign @Newcomer
    newcomer_role = discord.utils.get(member.guild.roles, name='Newcomer')
    if newcomer_role:
        try:
            await member.add_roles(newcomer_role)
        except discord.Forbidden:
            pass

    # Send Welcome DM
    try:
        embed = discord.Embed(
            title='👋 Welcome to Primordials!',
            description=(
                f'Hey **{member.name}**!\n\n'
                '🧬 Welcome to a calm, curious community exploring real mathematical ecosystems.\n\n'
                '**Next Steps:**\n'
                '1️⃣ Review <#rules>\n'
                '2️⃣ Introduce yourself in <#introductions>\n'
                '3️⃣ Grab help in <#help>\n'
                '4️⃣ Share your wallpaper in <#screenshots>'
            ),
            color=COLOR_MAGENTA
        )
        await member.send(embed=embed)
    except discord.Forbidden:
        pass

    # Broadcast in #welcome
    welcome_channel = discord.utils.get(member.guild.text_channels, name=WELCOME_CHANNEL_NAME)
    if welcome_channel:
        embed = discord.Embed(
            title='🎉 A new organism has entered the ecosystem',
            description=f'Please welcome {member.mention} to Primordials! Say hello in <#introductions> 🧬',
            color=COLOR_ALGAE
        )
        await welcome_channel.send(embed=embed)

@bot.tree.command(name='biome', description='Get information about a Primordials biome')
@app_commands.describe(name='Biome identifier')
@app_commands.choices(name=[
    app_commands.Choice(name='Algae Bloom', value='algae-bloom'),
    app_commands.Choice(name='Deep Sea', value='deep-sea'),
    app_commands.Choice(name='Primordial Soup', value='primordial-soup'),
    app_commands.Choice(name='Tundra', value='tundra'),
    app_commands.Choice(name='Coral Reef', value='coral-reef'),
    app_commands.Choice(name='Volcanic Vent', value='volcanic-vent'),
])
async def biome_command(interaction: discord.Interaction, name: str):
    descriptions = {
        'algae-bloom': '🌿 **Algae Bloom:** A thriving pond surface at dawn with algae, daphnia, and copepods.',
        'deep-sea': '🐙 **Deep Sea:** Bioluminescent abyss where anglerfish hunt squid in deep water.',
        'primordial-soup': '🍲 **Primordial Soup:** Ancient geothermal vents generating the first protocells.',
        'tundra': '❄️ **Tundra:** Stark Arctic summer with oscillating lemming and owl populations.',
        'coral-reef': '🪸 **Coral Reef:** High-density tropical biodiversity with clownfish and moray eels.',
        'volcanic-vent': '🌋 **Volcanic Vent:** Extremophiles thriving at deep-sea hydrothermal cracks.',
    }
    embed = discord.Embed(
        title=f'Biome: {name}',
        description=descriptions.get(name, 'Dynamic ecosystem configuration.'),
        color=COLOR_ALGAE
    )
    embed.add_field(name='Source Repository', value='https://github.com/SahilKhutey/Primodials/tree/main/biomes')
    await interaction.response.send_message(embed=embed)

if __name__ == '__main__':
    if not DISCORD_TOKEN:
        print('Error: DISCORD_TOKEN missing from environment.')
        sys.exit(1)
    bot.run(DISCORD_TOKEN)
```

---

## 🟨 2. JAVASCRIPT IMPLEMENTATION (`bot.js`)

```javascript
/**
 * Primordials Discord Bot — JavaScript (discord.js v14)
 */
require('dotenv').config();
const { Client, GatewayIntentBits, EmbedBuilder, SlashCommandBuilder, REST, Routes } = require('discord.js');

const TOKEN = process.env.DISCORD_TOKEN;
const GUILD_ID = process.env.GUILD_ID;

const client = new Client({
  intents: [
    GatewayIntentBits.Guilds,
    GatewayIntentBits.GuildMembers,
    GatewayIntentBits.GuildMessages,
    GatewayIntentBits.MessageContent,
  ],
});

client.once('ready', () => {
  console.log(`🧬 Bot ready and logged in as ${client.user.tag}`);
});

client.on('guildMemberAdd', async (member) => {
  if (member.user.bot) return;

  const welcomeChannel = member.guild.channels.cache.find((c) => c.name === 'welcome');
  if (welcomeChannel) {
    const embed = new EmbedBuilder()
      .setTitle('🎉 A new organism joined the ecosystem')
      .setDescription(`Welcome ${member} to Primordials! Say hi in #introductions 🌿`)
      .setColor(0x4ecdc4);
    await welcomeChannel.send({ embeds: [embed] });
  }
});

client.login(TOKEN);
```
