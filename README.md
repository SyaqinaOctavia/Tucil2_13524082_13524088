# Tugas Kecil 2 IF2211 Strategi Algoritma 2026

---

## Authors

Syaqina Octavia Rizha | 13524088 

---

## Deskripsi

Program ini dapat mengkonversi model .obj yang umum menjadi model .obj yang terdiri dari voxel dengan memanfaatkan properti Octree. Program melakukan konversi model dengan menggunakan algoritma divide and conquer. Strategi ini diimplementasikan dalam fungsi `BuildOctree`. Program dibuat dengan bahasa sehingga diperlukan compiler yang sesuai


## Project Structure

```
.
├── README.md               # File ini
├── bin                     # Executable file yang dapat digunakan langsung untuk menjalankan program
├── doc                     # Laporan penjelasan algoritma tugas ini
├── src                     # Kode utama untuk menjalankan program ini
├── test                    # File yang digunakan untuk testing, input serta outputnya
```

---

## Requirement
Compiler bahasa c++, yaitu g++

## Configuration

Jika ingin langsung menjalankan program bisa pergi ke root dan jalankan
`./bin/main`

Jika ingin mengubah kode dan mengkompilasi ulang, bisa pergi ke folder `src`
```
cd src
g++ helper.cpp vertex.cpp octreeNode.cpp main.cpp faceBuilder.cpp io.cpp -o ../bin/main
```

lalu run program
`../bin/main`
