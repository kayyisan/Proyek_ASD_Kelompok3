#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. STRUKTUR DATA BENTUKAN
// ==========================================

struct DataReservasi {
    string namaTim;
    string namaPenanggungJawab;
    string pilihanRuang;
    string jamMulai;
    string jamSelesai;
};

struct Simpul {
    DataReservasi info;
    Simpul* sambungan; 
};

// ==========================================
// 2. IMPLEMENTASI QUEUE (ANTREAN TUNGGU)
// ==========================================
struct AntreanTunggu {
    Simpul* kepala = nullptr; 
    Simpul* ekor = nullptr;   

    void tambahPendaftar(DataReservasi dataBaru) {
        Simpul* simpulBaru = new Simpul{dataBaru, nullptr};
        if (kepala == nullptr) {
            kepala = ekor = simpulBaru;
        } else {
            ekor->sambungan = simpulBaru;
            ekor = simpulBaru;
        }
    }

    DataReservasi prosesPendaftar() {
        if (kepala == nullptr) return {"", "", "", "", ""};
        
        Simpul* targetHapus = kepala;
        DataReservasi dataDiambil = targetHapus->info;
        
        kepala = targetHapus->sambungan;
        if (kepala == nullptr) ekor = nullptr; 
        
        delete targetHapus;
        return dataDiambil;
    }

    bool cekKosong() { return kepala == nullptr; }

    void cetakLayar() {
        if (cekKosong()) {
            cout << "  -> [Kosong] Belum ada tim di daftar tunggu.\n";
            return;
        }
        Simpul* jalan = kepala;
        int nomor = 1;
        while (jalan != nullptr) {
            cout << "  " << nomor++ << ". [" << jalan->info.pilihanRuang << "] Tim: " << jalan->info.namaTim 
                 << " | Pukul: " << jalan->info.jamMulai << "-" << jalan->info.jamSelesai << "\n";
            jalan = jalan->sambungan;
        }
    }
};

// ==========================================
// 3. IMPLEMENTASI STACK (RIWAYAT DISETUJUI)
// ==========================================
struct TumpukanRiwayat {
    Simpul* puncak = nullptr; 

    void simpanLog(DataReservasi dataDisetujui) {
        Simpul* simpulBaru = new Simpul{dataDisetujui, puncak};
        puncak = simpulBaru;
    }

    DataReservasi intipTerbaru() {
        if (puncak == nullptr) return {"", "", "", "", ""};
        return puncak->info;
    }

    bool cekKosong() { return puncak == nullptr; }

    void cetakLayar() {
        if (cekKosong()) {
            cout << "  -> [Kosong] Belum ada reservasi yang disetujui.\n";
            return;
        }
        Simpul* jalan = puncak;
        int nomor = 1;
        while (jalan != nullptr) {
            cout << "  [OK] " << nomor++ << ". " << jalan->info.namaTim 
                 << " (PJ: " << jalan->info.namaPenanggungJawab << ")\n"
                 << "       Ruang: " << jalan->info.pilihanRuang << " | " 
                 << jalan->info.jamMulai << " s/d " << jalan->info.jamSelesai << "\n";
            jalan = jalan->sambungan;
        }
    }
};

// ==========================================
// 4. PROGRAM UTAMA (MENU BERCABANG)
// ==========================================
int main() {
    AntreanTunggu sistemAntrean;
    TumpukanRiwayat sistemRiwayat;
    string modeAkses;

    while (true) {
        cout << "\n============================================\n";
        cout << "         SISTEM RESERVASI RUANG UGM         \n";
        cout << "============================================\n";
        cout << "Pilih Akses Login:\n";
        cout << "[1] Masuk sebagai Peserta (Ajukan Ruang)\n";
        cout << "[2] Masuk sebagai Admin (Kelola Antrean)\n";
        cout << "[0] Matikan Sistem\n";
        cout << "Masukkan pilihan (1/2/0): ";
        cin >> modeAkses;
        cin.ignore(); 

        // -----------------------------------------
        // TAMPILAN PESERTA
        // -----------------------------------------
        if (modeAkses == "1") {
            string aksiPeserta;
            while (true) {
                cout << "\n+--- PORTAL PESERTA ---+\n";
                cout << "[A] Buat Pengajuan Reservasi\n";
                cout << "[K] Kembali ke Menu Utama\n";
                cout << "Pilih: ";
                cin >> aksiPeserta;
                cin.ignore();

                if (aksiPeserta == "A" || aksiPeserta == "a") {
                    DataReservasi tiketBaru;
                    string opsiRuang;
                    bool ruangValid = false;

                    cout << "\n-- Form Pendaftaran --\n";
                    cout << "Nama Kelompok     : "; getline(cin, tiketBaru.namaTim);
                    cout << "Nama PJ           : "; getline(cin, tiketBaru.namaPenanggungJawab);
                    
                    // Looping pemilihan ruang sampai user mengisi dengan benar
                    while (!ruangValid) {
                        cout << "\nPilih Ruang yang Tersedia:\n";
                        cout << "1. Ruang Kolaborasi 1\n";
                        cout << "2. Ruang Kolaborasi 2\n";
                        cout << "3. Studio Presentasi\n";
                        cout << "Pilihan Anda (1/2/3): ";
                        cin >> opsiRuang;
                        cin.ignore();

                        if (opsiRuang == "1") { tiketBaru.pilihanRuang = "Ruang Kolaborasi 1"; ruangValid = true; }
                        else if (opsiRuang == "2") { tiketBaru.pilihanRuang = "Ruang Kolaborasi 2"; ruangValid = true; }
                        else if (opsiRuang == "3") { tiketBaru.pilihanRuang = "Studio Presentasi"; ruangValid = true; }
                        else { cout << "!! Pilihan tidak valid, silakan coba lagi.\n"; }
                    }

                    cout << "\nJam Mulai (HH:MM) : "; getline(cin, tiketBaru.jamMulai);
                    cout << "Jam Selesai       : "; getline(cin, tiketBaru.jamSelesai);
                    
                    sistemAntrean.tambahPendaftar(tiketBaru);
                    cout << ">> Berhasil! Pengajuan Anda masuk ke daftar tunggu untuk divalidasi Admin.\n";

                } else if (aksiPeserta == "K" || aksiPeserta == "k") {
                    break; // Keluar dari portal peserta
                } else {
                    cout << "!! Perintah tidak dikenali.\n";
                }
            }
        } 
        // -----------------------------------------
        // TAMPILAN ADMIN
        // -----------------------------------------
        else if (modeAkses == "2") {
            string aksiAdmin;
            while (true) {
                cout << "\n+--- PORTAL ADMIN ---+\n";
                cout << "[1] Validasi Antrean Terdepan\n";
                cout << "[2] Cek Info Reservasi Terakhir\n";
                cout << "[3] Monitor Papan Antrean (Queue)\n";
                cout << "[4] Lihat Riwayat Disetujui (Stack)\n";
                cout << "[0] Logout (Kembali)\n";
                cout << "Pilih: ";
                cin >> aksiAdmin;
                cin.ignore();

                if (aksiAdmin == "1") {
                    if (sistemAntrean.cekKosong()) {
                        cout << "\n>> Info: Tidak ada pengajuan di daftar tunggu.\n";
                    } else {
                        DataReservasi tiketDiproses = sistemAntrean.prosesPendaftar();
                        sistemRiwayat.simpanLog(tiketDiproses);
                        cout << "\n>> SUKSES: Reservasi untuk tim '" << tiketDiproses.namaTim << "' di " << tiketDiproses.pilihanRuang << " telah disetujui.\n";
                    }
                } 
                else if (aksiAdmin == "2") {
                    if (sistemRiwayat.cekKosong()) {
                        cout << "\n>> Info: Log riwayat masih bersih.\n";
                    } else {
                        DataReservasi terakhir = sistemRiwayat.intipTerbaru();
                        cout << "\n-- Validasi Terakhir --\n";
                        cout << "Tim   : " << terakhir.namaTim << "\n";
                        cout << "Ruang : " << terakhir.pilihanRuang << "\n";
                        cout << "Waktu : " << terakhir.jamMulai << " - " << terakhir.jamSelesai << "\n";
                    }
                } 
                else if (aksiAdmin == "3") {
                    cout << "\n-- Papan Daftar Tunggu --\n";
                    sistemAntrean.cetakLayar();
                } 
                else if (aksiAdmin == "4") {
                    cout << "\n-- Log Riwayat Validasi --\n";
                    sistemRiwayat.cetakLayar();
                } 
                else if (aksiAdmin == "0") {
                    break; // Keluar dari portal admin
                } 
                else {
                    cout << "!! Perintah tidak dikenali.\n";
                }
            }
        } 
        // -----------------------------------------
        // KELUAR
        // -----------------------------------------
        else if (modeAkses == "0") {
            cout << "\nSistem dimatikan. Terima kasih!\n";
            break;
        } 
        else {
            cout << "\n!! Opsi tidak valid. Masukkan 1, 2, atau 0.\n";
        }
    }

    return 0;
}