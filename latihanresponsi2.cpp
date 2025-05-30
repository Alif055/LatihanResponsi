#include <iostream>
#include <string>
using namespace std;
//fitur kurang
// undo
// remove dr playlist & daftar tapi bukan dr histori
struct Video {
    string judul;
    int durasi;
    string status;
    Video* l;
    Video* r;
};

struct nodeplist {
    Video* video;
    nodeplist* next;
};

struct nodeh {
    Video* video;
    nodeh* next;
};

Video* vr = nullptr;
nodeplist* pala = nullptr;
nodeplist* ekor = nullptr;
nodeh* palah = nullptr;

Video* tambah(string judul, int durasi) {
    Video* newVideo = new Video;
    newVideo->judul = judul;
    newVideo->durasi = durasi;
    newVideo->status = "tersedia";
    newVideo->l = nullptr;
    newVideo->r = nullptr;
    return newVideo;
}

// Fungsi untuk menambahkan video ke BST
Video* ptb(Video* root, Video* newVideo) {
    if (root == nullptr) {
        return newVideo;
    }
    
    if (newVideo->judul < root->judul) {
        root->l = ptb(root->l, newVideo);
    } else if (newVideo->judul > root->judul) {
        root->r = ptb(root->r, newVideo);
    }
    
    return root;
}

// Fungsi untuk menampilkan video secara inorder (ascending)
void daftar(Video* root) {
    if (root != nullptr) {
        daftar(root->l);
        cout << "Judul: " << root->judul << ", Durasi: " << root->durasi 
             << " menit, Status: " << root->status << endl;
        daftar(root->r);
    }
}

// menelusuri ptbnya***
Video* telusur(Video* root, string judul) {
    if (root == nullptr || root->judul == judul) {
        return root;
    }
    
    if (judul < root->judul) {
        return telusur(root->l, judul);
    } else {
        return telusur(root->r, judul);
    }
}

// Fungsi untuk menambahkan video ke playlist
void addToPlaylist(Video* video) {
    if (video->status != "tersedia") {
        cout << "Video tidak bisa ditambahkan ke playlist." << endl;
        return;
    }
    
    nodeplist* newNode = new nodeplist;
    newNode->video = video;
    newNode->next = nullptr;
    
    if (pala == nullptr) {
        pala = newNode;
        ekor = newNode;
        video->status = "sedang diputar";
    } else {
        ekor->next = newNode;
        ekor = newNode;
        video->status = "dalam antrean";
    }
    
    cout << "Video '" << video->judul << "' berhasil ditambahkan ke playlist." << endl;
}


void tonton() {
    if (pala == nullptr) {
        cout << "Tidak ada video dalam playlist." << endl;
        return;
    }
    
    Video* curr = pala->video;
    cout << "Menonton video: " << curr->judul << endl;
    
    // hist
    nodeh* newHistory = new nodeh;
    newHistory->video = curr;
    newHistory->next = palah;
    palah = newHistory;
    
    // rem
    nodeplist* temp = pala;
    pala = pala->next;
    delete temp;
    
    // statusquo
    curr->status = "tersedia";
    
    // autoplay
    if (pala != nullptr) {
        pala->video->status = "sedang diputar";
    }
}

void histori() {
    if (palah == nullptr) {
        cout << "Riwayat tontonan kosong." << endl;
        return;
    }
    
    cout << "Riwayat Tontonan:" << endl;
    nodeh* current = palah;
    while (current != nullptr) {
        cout << "Judul: " << current->video->judul << ", Durasi: " 
             << current->video->durasi << " menit" << endl;
        current = current->next;
    }
}
//dari bst
Video* del(Video* root, string judul) {
    if (root == nullptr) return root;
    
    if (judul < root->judul) {
        root->l = del(root->l, judul);
    } else if (judul > root->judul) {
        root->r = del(root->r, judul);
    } else {
        if (root->l == nullptr) {
            Video* bantu = root->r;
            delete root;
            return bantu;
        } else if (root->r == nullptr) {
            Video* bantu = root->l;
            delete root;
            return bantu;
        }
        
        Video* bantu = root->r;
        while (bantu->l != nullptr) {
            bantu = bantu->l;
        }
        
        root->judul = bantu->judul;
        root->durasi = bantu->durasi;
        root->status = bantu->status;
        
        root->r = del(root->r, bantu->judul);
    }
    return root;
}

// dari plist
void remo(string judul) {
    nodeplist* bantu = pala;
    nodeplist* prev = nullptr;
    
    while (bantu != nullptr && bantu->video->judul != judul) {
        prev = bantu;
        bantu = bantu->next;
    }
    
    if (bantu == nullptr) return; // Video tidak ditemukan di playlist
    
    if (prev == nullptr) {
        pala = bantu->next;
    } else {
        prev->next = bantu->next;
    }
    
    if (bantu == ekor) {
        ekor = prev;
    }
    
    delete bantu;
}

// void undoL() {
//     if (act == "") {
//         cout << "Tidak ada tindakan untuk di-undo." << endl;
//         return;
//     }
// }

int main() {
    int pil;
    string judul;
    int durasi;
    
    while (true) {
        cout << "\nMenu IDLIX Tube:" << endl;
        cout << "1. Tambah Video" << endl;
        cout << "2. Tampilkan Daftar Video" << endl;
        cout << "3. Tambahkan ke Playlist" << endl;
        cout << "4. Tonton Video" << endl;
        cout << "5. Riwayat Tontonan" << endl;
        cout << "6. Hapus Video" << endl;
        cout << "7. Undo Aksi Terakhir" << endl;
        cout << "8. Keluar" << endl;
        cout << "Pilihan: ";
        cin >> pil;
        cin.ignore();
        
        switch (pil) {
            case 1:
                cout << "Masukkan judul video: ";
                getline(cin, judul);
                cout << "Masukkan durasi video (menit): ";
                cin >> durasi;
                
                if (telusur(vr, judul) != nullptr) {
                    cout << "Video dengan judul tersebut sudah ada." << endl;
                } else {
                    Video* newVideo = tambah(judul, durasi);
                    vr = ptb(vr, newVideo);
                    cout << "Video berhasil ditambahkan." << endl;
                }
                break;
                
            case 2:
                if (vr == nullptr) {
                    cout << "Belum ada video." << endl;
                } else {
                    daftar(vr);
                    
                    char searchChoice;
                    cout << "Apakah Anda ingin mencari video (y/t)? ";
                    cin >> searchChoice;
                    cin.ignore();
                    
                    if (searchChoice == 'y' || searchChoice == 'Y') {
                        cout << "Masukkan judul video yang dicari: ";
                        getline(cin, judul);
                        Video* foundVideo = telusur(vr, judul);
                        if (foundVideo != nullptr) {
                            cout << "Video ditemukan:" << endl;
                            cout << "Judul: " << foundVideo->judul << ", Durasi: " 
                                 << foundVideo->durasi << " menit, Status: " 
                                 << foundVideo->status << endl;
                        } else {
                            cout << "Video tidak ditemukan." << endl;
                        }
                    }
                }
                break;
                
            case 3:
                if (vr == nullptr) {
                    cout << "Belum ada video." << endl;
                } else {
                    daftar(vr);
                    cout << "Masukkan judul video yang ingin ditambahkan ke playlist: ";
                    getline(cin, judul);
                    
                    Video* selectedVideo = telusur(vr, judul);
                    if (selectedVideo == nullptr) {
                        cout << "Video tidak ditemukan." << endl;
                    } else {
                        addToPlaylist(selectedVideo);
                    }
                }
                break;
                
            case 4:
                tonton();
                break;
                
            case 5:
                histori();
                break;
                
            case 6:
                if (vr == nullptr) {
                    cout << "Belum ada video." << endl;
                } else {
                    daftar(vr);
                    cout << "Masukkan judul video yang ingin dihapus: ";
                    getline(cin, judul);
                    
                    Video* videoToDelete = telusur(vr, judul);
                    if (videoToDelete == nullptr) {
                        cout << "Video tidak ditemukan." << endl;
                    } else {
                        if (videoToDelete->status != "tersedia") {
                            char confirm;
                            cout << "Video yang ingin dihapus [" << videoToDelete->status << "]. Yakin untuk tetap menghapus? (y/t) ";
                            cin >> confirm;
                            cin.ignore();
                            
                            if (confirm != 'y' && confirm != 'Y') {
                                cout << "Penghapusan dibatalkan." << endl;
                                break;
                            }
                        }
                        
                        remo(judul);
                        
                        vr = del(vr, judul);
                        cout << "Video berhasil dihapus." << endl;
                    }
                }
                break;

            case 7:
                cout << "fitur belum tersedia" << endl;
                break; //kurang ngerti cara implementasinya kak
                
            case 8: // gg
                cout << "Keluar dari program." << endl;
                return 0;
                
            default:
                cout << "Pilihan tidak valid." << endl;
        }
    }
    
    return 0;
}
