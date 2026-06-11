# Panduan Setup MCP — Claude Code + Unreal Engine 5.7

## Cara Kerja

```
Claude Code (kamu ketik perintah)
        ↓  MCP Protocol (stdio)
  Python Server (unreal_mcp_server.py)
        ↓  TCP Port 55557
  Plugin UnrealMCP (di dalam UE Editor)
        ↓
  Unreal Engine 5.7 — Level, Blueprint, Actor
```

---

## Setup Pertama Kali (Lakukan Sekali)

### Langkah 1 — Compile Plugin di Unreal Engine

1. Buka `d:\Game Tubes\Gameku\Gameku.uproject`
2. UE akan tanya "Plugin UnrealMCP butuh dikompilasi, mau kompilasi sekarang?" → klik **Yes**
3. Tunggu kompilasi selesai (5-10 menit pertama kali)
4. Editor akan terbuka

### Langkah 2 — Verifikasi Plugin Aktif

Di UE Editor:
- Buka **Edit → Plugins**
- Cari "UnrealMCP" → pastikan centang **Enabled**
- Restart Editor jika diminta

### Langkah 3 — Aktifkan Remote Control Plugin

Di UE Editor → **Edit → Plugins** → cari "Remote Control" → Enable

---

## Cara Pakai Setiap Session

### Di Terminal (sebelum buka Claude Code):
```
# Tidak perlu start server manual — Claude Code akan menjalankan
# Python MCP server secara otomatis via stdio
```

### Di Unreal Engine:
Pastikan UE Editor sudah buka dan project Gameku aktif.
Plugin UnrealMCP akan auto-start TCP listener di port 55557 saat UE buka.

### Di Claude Code:
Buka folder `d:\Game Tubes` di Claude Code — MCP server `unreal-mcp` akan aktif otomatis.

---

## Contoh Perintah yang Bisa Kamu Minta ke Claude

Setelah MCP aktif, kamu bisa minta saya langsung:

### Spawn Actor
```
"Spawn sebuah BP_FishingSpot di koordinat X=1000, Y=500"
"Taruh 5 FishingSpot di sekitar origin"
```

### Edit Blueprint
```
"Buat Blueprint baru bernama BP_KakabMerah turunan dari FishBase"
"Tambahkan variable CurrentBaitID ke BP_FishingCharacter"
```

### Level Management
```
"Pindahkan semua FishingSpot ke layer 'Fishing_Spots'"
"Hapus semua actor dengan tag 'Temporary'"
```

### Material & Asset
```
"Assign material M_Ocean ke semua StaticMesh yang bernama Beach*"
```

### Automation
```
"Jalankan Python script untuk spawn ikan di semua FishingSpot"
"Compile semua Blueprint yang ada error"
```

---

## Troubleshooting

### MCP tidak terdeteksi di Claude Code
- Pastikan berada di folder `d:\Game Tubes` (bukan subfolder)
- Restart Claude Code
- Cek file `d:\Game Tubes\.mcp.json` ada dan benar

### "Connection refused" / Python server error
- Pastikan UE Editor sedang buka dengan project Gameku
- Cek plugin UnrealMCP aktif di UE
- Port 55557 harus bebas (tidak dipakai aplikasi lain)

### Plugin tidak mau compile
- Pastikan Visual Studio 2022 terinstall dengan komponen "Game development with C++"
- Atau gunakan Rider for Unreal Engine

---

## File Konfigurasi

| File | Lokasi | Fungsi |
|------|--------|--------|
| `.mcp.json` | `d:\Game Tubes\` | Definisi MCP server |
| `.claude/settings.json` | `d:\Game Tubes\` | Auto-approve MCP server |
| `unreal_mcp_server.py` | `C:\Users\raiha\unreal-mcp\Python\` | Python MCP server |
| `UnrealMCP.uplugin` | `Gameku\Plugins\UnrealMCP\` | UE Plugin |
