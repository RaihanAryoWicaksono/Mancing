# Game Design Document
# NELAYAN - Indonesian Ocean Fishing Simulator

**Version:** 1.0  
**Engine:** Unreal Engine 5.7  
**Platform:** PC (Windows), Console (future)  
**Genre:** Fishing Simulator / Open World / Adventure  
**Target Audience:** 13+ (Casual to Hardcore Gamers)  
**Developer:** Game Tubes  

---

## DAFTAR ISI

1. [Game Overview](#1-game-overview)
2. [Core Gameplay Loop](#2-core-gameplay-loop)
3. [Fishing Mechanics](#3-fishing-mechanics)
4. [Fish Species & Database](#4-fish-species--database)
5. [World & Environment](#5-world--environment)
6. [Boat System](#6-boat-system)
7. [Economy System](#7-economy-system)
8. [Progression System](#8-progression-system)
9. [Quest & Story System](#9-quest--story-system)
10. [UI/UX Design](#10-uiux-design)
11. [Audio Design](#11-audio-design)
12. [Technical Architecture](#12-technical-architecture)
13. [Monetization & Scope](#13-monetization--scope)
14. [Milestone & Roadmap](#14-milestone--roadmap)

---

## 1. GAME OVERVIEW

### 1.1 Konsep Utama
**NELAYAN** adalah game simulasi memancing open-world yang terinspirasi dari kehidupan nelayan Indonesia. Pemain memulai sebagai nelayan muda di desa kecil dan bertumbuh menjadi raja lautan dengan armada kapal dan bisnis perikanan yang besar.

### 1.2 Unique Selling Points (USP)
- **Setting Indonesia asli** — laut Nusantara, ikan lokal, budaya nelayan Indonesia
- **Fishing mini-game mendalam** — bukan sekadar klik, ada sistem tensi, trik, dan taktik
- **Ekosistem hidup** — ikan berperilaku berbeda di waktu, cuaca, dan musim berbeda
- **Ekonomi dinamis** — harga ikan fluktuatif seperti pasar nyata
- **Boat progression** — dari perahu dayung sampai kapal besar bermesin

### 1.3 Pillars of Design
| Pilar | Deskripsi |
|-------|-----------|
| **Authenticity** | Rasakan pengalaman nelayan Indonesia yang nyata |
| **Depth** | Sistem memancing yang kaya tapi mudah dipelajari |
| **Discovery** | Selalu ada ikan baru, spot baru, dan rahasia untuk ditemukan |
| **Progression** | Pertumbuhan yang terasa bermakna dari nelayan kecil ke raja lautan |

### 1.4 Referensi Game
- Stardew Valley (progression & relaxing gameplay)
- Fishing Planet (fishing mechanics depth)
- Dredge (atmosfer & exploration)
- Sea of Thieves (ocean traversal)
- Dave the Diver (dual gameplay loop)

---

## 2. CORE GAMEPLAY LOOP

```
┌─────────────────────────────────────────────────────────┐
│                    CORE LOOP HARIAN                      │
│                                                         │
│  [PERSIAPAN]                                            │
│   • Cek cuaca & pasang surut hari ini                   │
│   • Beli umpan & perlengkapan                           │
│   • Pilih spot memancing                                │
│        ↓                                                │
│  [PERJALANAN]                                           │
│   • Navigasi kapal ke spot                              │
│   • Hindari badai / arus kuat                           │
│   • Temukan spot baru di perjalanan                     │
│        ↓                                                │
│  [MEMANCING]                                            │
│   • Cast rod ke titik optimal                           │
│   • Tunggu / gunakan teknik attract                     │
│   • Mini-game catch (reeling, tension)                  │
│   • Simpan ikan ke palka kapal                          │
│        ↓                                                │
│  [KEMBALI & JUAL]                                       │
│   • Kembali ke pelabuhan                                │
│   • Jual / proses ikan                                  │
│   • Upgrade peralatan & kapal                           │
│        ↓                                                │
│  [PROGRESSION]                                          │
│   • Level naik, skill baru                              │
│   • Unlock area baru                                    │
│   • Selesaikan quest                                    │
└─────────────────────────────────────────────────────────┘
```

### 2.1 Session Loop
- **Short Session (15-30 menit):** Pergi ke spot dekat, tangkap beberapa ikan, kembali jual
- **Medium Session (1-2 jam):** Eksplorasi zone baru, selesaikan quest, upgrade
- **Long Session (3+ jam):** Ekspedisi laut dalam, kompetisi memancing, story mission

---

## 3. FISHING MECHANICS

### 3.1 Rod & Casting System

**Tipe Joran (Rod):**
| ID | Nama | Range Cast | Kekuatan | Cocok Untuk |
|----|------|-----------|----------|-------------|
| R01 | Joran Bambu | 5-15m | Rendah | Ikan kecil, pantai |
| R02 | Joran Spinning | 10-30m | Sedang | Ikan sedang, danau |
| R03 | Joran Casting | 15-40m | Sedang-Tinggi | Ikan predator |
| R04 | Joran Trolling | - (taruh di holder) | Tinggi | Speed fishing di kapal |
| R05 | Joran Jigging | Dalam (50-200m) | Sangat Tinggi | Ikan laut dalam |
| R06 | Joran Surfcasting | 40-80m | Tinggi | Pantai, ombak besar |
| R07 | Joran Tegek | 2-8m | Rendah | Sungai, spot sempit |

**Casting Mechanics:**
- Hold & release untuk tenaga cast (power bar)
- Aiming dengan reticle yang dipengaruhi angin
- Landing zone indicator (optimal zone = bonus)
- Casting skill mempengaruhi akurasi

### 3.2 Bait & Lure System

**Tipe Umpan:**
| Kategori | Contoh | Efek |
|----------|--------|------|
| Hidup | Cacing, Udang, Ikan kecil | Paling efektif, habis pakai |
| Mati | Cumi-cumi, Ikan asin | Efektif, lebih awet |
| Artificial | Lure, Jig, Popper | Reusable, butuh teknik |
| Special | Umpan Racikan, Essence | Buff sementara, rare fish |

**Bait Effectiveness Matrix:**
- Setiap ikan punya preferensi umpan
- Musim dan waktu mempengaruhi efektivitas
- Air temperature mempengaruhi aktivitas ikan

### 3.3 Fishing Mini-Game

**Fase 1 — Cast & Wait:**
- Ikan AI berenang dan mendeteksi umpan
- Indikator tension yang naik saat ikan tertarik
- Player bisa "jiggle" untuk attract

**Fase 2 — Bite Detection:**
- Visual: Pelampung bergerak / tenggelam
- Audio: Suara klik reel
- Haptic: Controller vibration
- Timing window untuk hook (terlambat = lepas)

**Fase 3 — Fighting / Reeling:**
```
┌─────────────────────────────────────┐
│  TENSION METER                      │
│  ░░░░░░▓▓▓▓▓▓▓█████████░░░░░░      │
│         ^SAFE    ^DANGER  ^BREAK    │
│                                     │
│  [←] Reel In    [→] Let Line       │
│  [↑] Pull Up    [↓] Lower Rod      │
│                                     │
│  Fish Stamina: ████████░░  80%      │
│  Line Strength: ████░░░░░░  40%    │
└─────────────────────────────────---|
```

**Fighting Behaviors Per Fish:**
- **Sprinter** (Tenggiri): Lari cepat, butuh line slack
- **Diver** (Kakap): Menyelam dalam, butuh rod tekukan
- **Jumper** (Marlin): Melompat, timing reel harus tepat
- **Shaker** (Layur): Geleng kepala, grip harus kuat
- **Steady** (Bawal): Bergerak stabil, battle lama

### 3.4 Fish Detection System
- Radar sonar opsional (untuk kapal besar)
- Bubble/surface indicator di area banyak ikan
- Bird gathering menunjukkan schooling fish
- Water color indicator (jernih = ikan karang, keruh = ikan sungai)
- Periscope view untuk lihat bawah air

### 3.5 Advanced Techniques
| Teknik | Deskripsi | Unlock Level |
|--------|-----------|-------------|
| Jigging | Gerak lure naik-turun | Level 5 |
| Trolling | Lempar sambil kapal jalan | Level 10 |
| Night Fishing | Gunakan lampu attract | Level 8 |
| Deep Sea | Fishing di kedalaman 200m+ | Level 15 |
| Fly Fishing | Cast ke sungai/mangrove | Level 12 |
| Dynamite (Illegal) | Instan tapi ada konsekuensi | — |

---

## 4. FISH SPECIES & DATABASE

### 4.1 Kategori Ikan

**IKAN PANTAI & PESISIR (Common)**
| ID | Nama Indonesia | Nama Latin | Berat Max | Nilai Jual | Habitat |
|----|---------------|-----------|-----------|-----------|---------|
| F001 | Ikan Bandeng | Chanos chanos | 5kg | Rp 25.000/kg | Tambak, Pantai |
| F002 | Ikan Kembung | Rastrelliger kanagurta | 0.5kg | Rp 30.000/kg | Pantai |
| F003 | Ikan Layang | Decapterus spp | 0.8kg | Rp 28.000/kg | Pantai |
| F004 | Ikan Selar | Selaroides leptolepis | 0.3kg | Rp 22.000/kg | Pantai |
| F005 | Ikan Teri | Stolephorus spp | 0.1kg | Rp 40.000/kg | Pantai |

**IKAN KARANG (Uncommon)**
| ID | Nama Indonesia | Nama Latin | Berat Max | Nilai Jual | Habitat |
|----|---------------|-----------|-----------|-----------|---------|
| F010 | Ikan Kakap Merah | Lutjanus campechanus | 15kg | Rp 85.000/kg | Karang |
| F011 | Ikan Kerapu Macan | Epinephelus fuscoguttatus | 20kg | Rp 120.000/kg | Karang |
| F012 | Ikan Kerapu Bebek | Cromileptes altivelis | 3kg | Rp 200.000/kg | Karang, Rare |
| F013 | Ikan Napoleon | Cheilinus undulatus | 50kg | Rp 500.000/kg | Karang, Rare |
| F014 | Ikan Baronang | Siganus guttatus | 1kg | Rp 45.000/kg | Karang |

**IKAN PELAGIS (Rare)**
| ID | Nama Indonesia | Nama Latin | Berat Max | Nilai Jual | Habitat |
|----|---------------|-----------|-----------|-----------|---------|
| F020 | Ikan Tongkol | Euthynnus affinis | 5kg | Rp 35.000/kg | Laut Terbuka |
| F021 | Ikan Cakalang | Katsuwonus pelamis | 10kg | Rp 40.000/kg | Laut Terbuka |
| F022 | Ikan Tenggiri | Scomberomorus commerson | 30kg | Rp 65.000/kg | Laut Terbuka |
| F023 | Ikan Tuna Sirip Kuning | Thunnus albacares | 200kg | Rp 90.000/kg | Laut Dalam |
| F024 | Ikan Marlin Biru | Makaira nigricans | 600kg | Rp 300.000/kg | Laut Dalam, Very Rare |

**IKAN UNIK & LEGENDARY**
| ID | Nama Indonesia | Nama Latin | Berat Max | Nilai Jual | Keterangan |
|----|---------------|-----------|-----------|-----------|-----------|
| F030 | Ikan Coelacanth | Latimeria menadoensis | 95kg | Rp 5.000.000/ekor | Fosil hidup, Sulawesi Utara |
| F031 | Ikan Arapaima | Arapaima gigas | 200kg | - | Quest only, sungai |
| F032 | Hiu Paus | Rhincodon typus | 2000kg | Dilarang ditangkap | Foto = reward |
| F033 | Penyu | Chelonia mydas | 150kg | Dilarang ditangkap | Foto = reward |
| F034 | Ikan Layur Raksasa | Regalecus glesne | 300kg | Rp 1.000.000/ekor | Legendary, sangat rare |

### 4.2 Fish AI Behavior
Setiap ikan memiliki:
- **Activity Hours:** Kapan ikan paling aktif (dawn, day, dusk, night)
- **Preferred Depth:** Kedalaman favorit
- **Temperature Range:** Suhu air optimal
- **Schooling Behavior:** Bergerombol atau soliter
- **Seasonal Migration:** Pola migrasi musiman
- **Aggression Level:** Seberapa agresif menyerang umpan

---

## 5. WORLD & ENVIRONMENT

### 5.1 Peta Dunia — Kepulauan Nusantara

```
     [LAUT ANDAMAN]
         ↑
[SABANG] ←── [ACEH] ──→ [LAUT NATUNA]
                                 ↓
          [KALIMANTAN COAST]
              ↓         ↓
[JAWA COAST] ──── [SELAT SUNDA] ──── [BALI]
     ↓                                    ↓
[LAUT SELATAN]                    [FLORES SEA]
                                         ↓
                              [BANDA SEA] ──→ [MALUKU]
                                    ↓
                             [ARAFURA SEA]
```

**Zone Progression:**
| Zone | Level Unlock | Tipe | Ciri Khas |
|------|-------------|------|-----------|
| Teluk Kampung | Mulai | Pantai | Tutorial area, aman |
| Perairan Dangkal | Level 3 | Karang | Ikan karang, snorkeling |
| Selat & Arus | Level 7 | Pelagis | Arus kuat, ikan besar |
| Lepas Pantai | Level 12 | Laut Dalam | Butuh kapal besar |
| Zona Terlarang | Level 20 | Laut Dalam Ekstrem | Legendary fish, bahaya |
| Palung Nusantara | Level 30 | Ultra Deep | Endgame content |

### 5.2 Settlement & Locations

**Pelabuhan Utama — Desa Bahari (Hub Utama)**
- Pasar ikan (jual ikan)
- Toko perlengkapan Pak Budi
- Bengkel kapal Bang Roni
- Rumah pemain
- Dermaga kapal
- Papan info quest
- Balai warga (story events)

**Lokasi Tambahan:**
- Pulau Karang (fishing spot premium)
- Mercusuar Tua (fast travel point)
- Pelabuhan Kota (lebih mahal, lebih banyak pilihan)
- Kampung Nelayan Terpencil (quest location)
- Bangkai Kapal (underwater exploration)

### 5.3 Weather System

**Cuaca & Efek:**
| Cuaca | Efek Memancing | Efek Kapal | Frekuensi |
|-------|---------------|-----------|-----------|
| Cerah | Normal | Normal | 40% |
| Berawan | +10% bite rate | Angin sedang | 25% |
| Hujan Ringan | +20% bite rate (ikan aktif) | Angin, ombak sedang | 20% |
| Badai | -50% bite rate | Berbahaya, balik ke port | 10% |
| Kabut | -30% visibility | Navigasi sulit | 5% |

**Sistem Pasang Surut:**
- High Tide: Ikan karang masuk lebih dalam ke pantai
- Low Tide: Spot baru terbuka, ikan konsentrasi di lubuk
- Spring Tide: Event khusus setiap bulan purnama

### 5.4 Time of Day System
- 1 hari game = 24 menit real
- Dawn (05:00-07:00): Peak bite time, ikan permukaan aktif
- Morning (07:00-10:00): Good bite time
- Day (10:00-14:00): Slow period, ikan masuk kedalaman
- Evening (14:00-18:00): Good bite time kembali
- Dusk (18:00-20:00): Peak bite time (ikan predator aktif)
- Night (20:00-05:00): Night fishing species aktif

---

## 6. BOAT SYSTEM

### 6.1 Tipe Kapal

| Tier | Nama | Kecepatan | Storage | Harga | Feature |
|------|------|-----------|---------|-------|---------|
| 1 | Rakit Bambu | 5 km/h | 10 kg | Gratis | Tutorial only |
| 2 | Perahu Dayung | 8 km/h | 30 kg | Rp 500.000 | Manual row |
| 3 | Perahu Layar | 12 km/h | 60 kg | Rp 2.000.000 | Sail, butuh angin |
| 4 | Perahu Motor Kecil | 20 km/h | 100 kg | Rp 8.000.000 | Engine, fuel |
| 5 | Kapal Ikan Sedang | 25 km/h | 500 kg | Rp 50.000.000 | Sonar, cooler box |
| 6 | Kapal Nelayan Besar | 30 km/h | 2000 kg | Rp 200.000.000 | Crane, deep sea eq |
| 7 | Kapal Armada | 35 km/h | 10000 kg | Rp 1.000.000.000 | Fleet management |

### 6.2 Boat Navigation
- WASD / Stick untuk steer
- Throttle system (0-100%)
- Autopilot ke waypoint yang sudah di-unlock
- Anchor system untuk berhenti di spot
- Perlu bahan bakar (solar/bensin)
- Boat damage system (badai, karang, dll)

### 6.3 Boat Upgrades
| Modul | Fungsi | Level Req |
|-------|--------|-----------|
| Fish Finder / Sonar | Deteksi ikan bawah air | 5 |
| Cooler Box | Storage ikan lebih awet | 3 |
| Live Bait Tank | Simpan umpan hidup | 4 |
| Autopilot | Navigasi otomatis | 8 |
| Outrigger | Trolling lebih efektif | 6 |
| Winch / Crane | Tarik ikan besar | 12 |
| Radar Cuaca | Prediksi cuaca akurat | 10 |
| Cabin | Bermalam di laut | 15 |

---

## 7. ECONOMY SYSTEM

### 7.1 Sumber Pendapatan
1. **Jual Ikan Langsung** — ke pasar ikan lokal
2. **Export Premium** — jual ke kota, harga lebih tinggi (+20-50%)
3. **Restoran Supply** — kontrak rutin, harga tetap tapi stabil
4. **Fishing Contest** — hadiah besar dari kompetisi
5. **Foto Rare Fish** — jual foto ke majalah/media
6. **Quest Reward** — dari NPC quest
7. **Treasure Found** — dari bangkai kapal dll

### 7.2 Sistem Harga Dinamis
- Harga ikan berubah berdasarkan supply lokal
- Overfishing di satu area = harga turun
- Musim tertentu = jenis ikan langka = harga premium
- Festival lokal = demand naik = harga naik

### 7.3 Pengeluaran
| Kategori | Contoh |
|----------|--------|
| Bahan Bakar | Solar kapal (per liter) |
| Umpan | Beli di toko setiap trip |
| Perbaikan | Kapal rusak, joran patah |
| Lisensi | Izin melaut di zone tertentu |
| Upgrade | Peralatan baru |
| Pajak | % dari pendapatan di late game |

---

## 8. PROGRESSION SYSTEM

### 8.1 Player Level & XP
XP didapat dari:
- Tangkap ikan (XP by rarity & size)
- Selesaikan quest
- Discover new area
- Catch first of each species
- Daily challenges

### 8.2 Skill Tree

```
                    [NELAYAN AGUNG]
                          ↑
        ┌─────────────────┼─────────────────┐
        ↑                 ↑                 ↑
  [AHLI MANCING]    [KAPTEN KAPAL]    [JURAGAN IKAN]
        ↑                 ↑                 ↑
  ┌─────┴──────┐    ┌─────┴──────┐   ┌─────┴──────┐
  │            │    │            │   │            │
[Casting]  [Sensing] [Navigation] [Maint] [Trade] [Business]
```

**Skill Branches:**

**CABANG MANCING:**
| Skill | Efek |
|-------|------|
| Power Cast Lv.1-5 | +10% jarak cast per level |
| Fishing Sense Lv.1-5 | Deteksi ikan lebih mudah |
| Iron Grip Lv.1-5 | +15% line strength |
| Quick Hook Lv.1-3 | Window hook lebih besar |
| Patience Lv.1-5 | -20% waktu tunggu bite |
| Night Vision Lv.1-3 | Efektivitas night fishing |
| Deep Sea Expert | Unlock deep sea fishing |

**CABANG KAPAL:**
| Skill | Efek |
|-------|------|
| Navigation | +speed, fuel efficiency |
| Storm Rider | Kapal tahan cuaca buruk |
| Sonar Expert | Sonar range & accuracy |
| Fleet Manager | Kelola multiple boat |

**CABANG BISNIS:**
| Skill | Efek |
|-------|------|
| Bargaining | +% harga jual |
| Market Sense | Lihat prediksi harga |
| Export License | Akses pasar premium |
| Business Empire | Kelola usaha perikanan |

### 8.3 Fish Encyclopedia
- 150+ spesies untuk ditemukan
- Setiap tangkapan baru = entry + lore
- Complete category = unlock bonus
- Complete all = title "Ahli Ikan Nusantara"

---

## 9. QUEST & STORY SYSTEM

### 9.1 Main Story — "Warisan Sang Nelayan"

**Act 1 — Pemula (Level 1-10)**
- Pemain baru datang ke Desa Bahari setelah kakek wafat
- Warisi perahu tua kakek
- Pelajari dasar memancing dari Pak Tua
- Selesaikan hutang kakek ke rentenir lokal
- Discover bahwa ada misteri di laut sekitar desa

**Act 2 — Petualangan (Level 11-20)**
- Jelajahi pulau-pulau sekitar
- Temukan nelayan tua yang tahu rahasia laut dalam
- Hadapi ancaman perusahaan ikan ilegal (illegal fishing)
- Build reputasi sebagai nelayan terbaik di kawasan

**Act 3 — Raja Lautan (Level 21-30)**
- Konfrontasi boss illegal fishing corporation
- Temukan legenda ikan kuno (Coelacanth quest)
- Unite komunitas nelayan lokal
- Bangun kerajaan perikanan yang berkelanjutan

**Act 4 — Endgame**
- Manage fleet & business empire
- Explore zona terlarang & palung dalam
- Hunt legendary fish
- Fishing World Championship

### 9.2 Side Quest Categories
| Kategori | Contoh |
|----------|--------|
| Delivery | Antar ikan segar ke restoran dalam waktu X |
| Hunting | Tangkap X ekor ikan jenis Y |
| Exploration | Temukan lokasi rahasia |
| Rescue | Selamatkan nelayan hilang |
| Environmental | Bersihkan sampah laut |
| Festival | Ikut lomba memancing desa |

### 9.3 Daily & Weekly Challenges
- **Harian:** Tangkap 3 ikan dengan berat total 10kg, dll
- **Mingguan:** Tangkap 1 ikan Rare, dll
- **Monthly:** Fishing tournament

---

## 10. UI/UX DESIGN

### 10.1 HUD In-Game
```
┌─────────────────────────────────────────────────────┐
│ [Waktu 06:23] [Cuaca: ⛅Berawan] [Angin: ↗ 12km/h] │
│                                                     │
│                  [GAMEPLAY VIEW]                    │
│                                                     │
│ Storage: ██░░  [Sonar Map]    Fuel: ████           │
│ 45/100 kg                     80%                  │
│                                                     │
│ [Rod: Spinning] [Bait: Udang x5] [Stamina: ████]   │
└─────────────────────────────────────────────────────┘
```

### 10.2 Menu Struktur
- **Inventory** — ikan, peralatan, material
- **Map** — world map, fast travel, waypoints
- **Skill Tree** — progression
- **Encyclopedia** — fish database
- **Quest Log** — active & completed
- **Boat Management** — kapal & upgrade
- **Market** — harga ikan saat ini
- **Settings** — grafis, audio, kontrol

### 10.3 Accessibility
- Font size adjustable
- Color blind mode
- Fishing mini-game difficulty (Easy/Normal/Hard)
- Auto-reel option untuk pemain casual
- Subtitle & terjemahan (Indonesia + English)

---

## 11. AUDIO DESIGN

### 11.1 Musik
| Situasi | Mood | Instrumen |
|---------|------|-----------|
| Desa / Hub | Tenang, hangat | Gamelan ringan, gitar |
| Perjalanan Laut | Petualangan | Orkestra, perkusi laut |
| Memancing | Santai, fokus | Ambient, suara air |
| Fish Fighting | Tegang | Dinamis, percussion |
| Badai | Dramatis | Orkestra besar |
| Malam | Mistis, tenang | Ambient, suling |
| Victory | Gembira | Fanfare, gamelan |

### 11.2 Sound Design
- Suara air laut authentic (recorded)
- Suara reel & joran
- Suara ikan di air
- Ambient burung, angin
- Suara NPC / dialog (Bahasa Indonesia)
- Sound effect UI yang responsif

---

## 12. TECHNICAL ARCHITECTURE

### 12.1 Sistem C++ Utama

```
Source/Gameku/
├── Core/
│   ├── GamekuGameMode          # Game rules, spawning
│   ├── GamekuGameInstance      # Persistent data, save/load
│   ├── GamekuPlayerController  # Input, camera, UI
│   └── GamekuSaveGame          # Save game data
│
├── Character/
│   ├── FishingCharacter        # Player pawn
│   └── NPCCharacter            # Villager, shop NPC
│
├── Fishing/
│   ├── FishingRodComponent     # Rod logic, cast, reel
│   ├── FishingLineComponent    # Line physics, tension
│   ├── FishingSpot             # Hotspot actor
│   ├── FishBase                # Base fish AI
│   ├── FishDataAsset           # Fish stats data asset
│   └── FishingMinigameComponent # Mini-game logic
│
├── Boat/
│   ├── FishingBoat             # Boat actor
│   ├── BoatMovementComponent   # Boat physics
│   └── BoatUpgradeComponent    # Upgrade management
│
├── Inventory/
│   ├── InventoryComponent      # Player inventory
│   ├── FishingItem             # Base item class
│   └── FishInstance            # Caught fish instance
│
├── Economy/
│   ├── FishMarketActor         # Market logic
│   ├── EconomyManager          # Price calculation
│   └── ContractSystem          # Supply contracts
│
├── Environment/
│   ├── WeatherSystem           # Weather controller
│   ├── TimeOfDayManager        # Day/night cycle
│   ├── OceanManager            # Tides, currents
│   └── FishingZone             # Area fish spawner
│
├── Progression/
│   ├── SkillTreeComponent      # Skill system
│   ├── QuestManager            # Quest tracking
│   ├── AchievementManager      # Achievements
│   └── EncyclopediaManager     # Fish database
│
└── UI/
    ├── FishingHUD              # Main HUD
    ├── MinigameWidget          # Fishing mini-game UI
    └── InventoryWidget         # Inventory UI
```

### 12.2 Blueprint Systems (Editor-Side)
- **BP_Fish_[Species]** — derived dari FishBase, override stats
- **BP_FishingSpot_[Type]** — derived dari FishingSpot
- **BP_Boat_[Tier]** — derived dari FishingBoat
- **BP_NPC_[Name]** — derived dari NPCCharacter
- **W_MainMenu, W_HUD, W_Inventory** — UI Widgets
- **DT_FishData** — DataTable untuk fish stats
- **DT_ItemData** — DataTable untuk items

### 12.3 Teknologi UE5 yang Dipakai
| Fitur UE5 | Penggunaan |
|-----------|-----------|
| Lumen | Global illumination, cahaya laut |
| Nanite | Detail high-poly ikan & lingkungan |
| Water Plugin | Simulasi air laut realistis |
| Chaos Physics | Simulasi tali pancing |
| Mass AI | Schooling fish behavior |
| MetaHuman | NPC berkualitas tinggi |
| World Partition | Open world seamless |
| PCG (Procedural Content Gen) | Vegetasi pantai, ikan spawning |

---

## 13. MONETIZATION & SCOPE

### 13.1 Model Bisnis
- **Buy to Play** — harga dasar ~Rp 150.000 - Rp 300.000
- **Expansion DLC** — area baru, ikan baru (Maluku Pack, Raja Ampat Pack)
- **Cosmetic DLC** — skin kapal, skin rod (tidak pay-to-win)

### 13.2 Post-Launch Content
- Free Update: Bug fix, balancing, 1-2 ikan baru
- Expansion: Area baru (Maluku, Papua, Sulawesi)
- Seasonal Event: Ramadan fishing festival, Hari Kemerdekaan tournament

---

## 14. MILESTONE & ROADMAP

### Pre-Production (Bulan 1-3)
- [ ] Finalisasi GDD
- [ ] Prototipe fishing mechanics
- [ ] Art style guide
- [ ] Prototipe world navigation
- [ ] Core C++ architecture

### Alpha (Bulan 4-9)
- [ ] Core fishing loop lengkap
- [ ] 20 spesies ikan
- [ ] 1 zone (Teluk Kampung)
- [ ] Basic economy
- [ ] Basic boat system

### Beta (Bulan 10-15)
- [ ] 4 zones
- [ ] 80 spesies ikan
- [ ] Full story Act 1 & 2
- [ ] Skill tree
- [ ] Weather system
- [ ] Boat upgrades

### Release Candidate (Bulan 16-18)
- [ ] All 6 zones
- [ ] 150+ spesies
- [ ] Full story
- [ ] Multiplayer consideration
- [ ] Polish & optimization
- [ ] QA testing

### Launch Target: Bulan 18-24

---

*GDD ini adalah dokumen hidup dan akan terus diperbarui selama development.*  
*Version 1.0 — Game Tubes, 2026*
