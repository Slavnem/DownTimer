// Slavnem @2024-07-21
// Dosya İşleme Yapısı Tanımlamaları
#ifndef DT_FILESTREAM_H
#define DT_FILESTREAM_H

// Kütüphaneler
#include "CompileConf.h"
#include <stdio.h>
#include "TypeDefine.h"
#include "Macro.h"

// Dosya İşleyicisi Hata Ayıklayıcısı Belirteci
#ifndef __COMPILE_RELEASE_MODE__
    #define __DEBUG_MSG_FILESTREAM__
#endif

// Dosya Türü Tanımlaması
typedef FILE file, *fileptr; // korumasız
typedef const file file_s; // basit korumalı
typedef const file* fileptr_a; // bellek adresinden değiştirilebilir
typedef const file* const fileptr_s; // hiçbir şekilde değiştirilemez

// Dosya Açma Türleri
typedef enum
{
    // herhangi bir şey yapmasın
    EFS_OTYPE_NULL = 0, // bir şey yapmasın

    // r = okur, dosya mevcut değilse hata verir
    EFS_OTYPE_READ = 10, // okuma
    EFS_OTYPE_READWRITE, // okuma ve yazma
    EFS_OTYPE_READBIN, // ikilik sistemde okuma
    EFS_OTYPE_READWRITEBIN, // ikilik sistemde okuma ve yazma

    // w = yazar, dosya yoksa oluşturur
    EFS_OTYPE_WRITE = 20, // yazma
    EFS_OTYPE_WRITEREAD, // okuma ve yazma
    EFS_OTYPE_WRITEBIN, // ikilik sistemde yazma
    EFS_OTYPE_WRITEREADBIN, // ikilik sistemde okuma ve yazma

    // a = dosya mevcutsa sona gder değilse oluşturur
    EFS_OTYPE_ADD = 30, // sondan eklemeye devam et
    EFS_OTYPE_ADDREAD, // okuma ve ekleme
    EFS_OTYPE_ADDBIN, // sondan eklemeye ikilik sistemde devam et, ekleme
    EFS_OTYPE_ADDREADBIN, // sondan eklemeye ikilik sistemde devam et, okuma ve ekleme
} efsotype;

typedef const efsotype efsotype_s;

// Dosya İşlem Kodları
typedef enum
{
    // Hatalar
    EFS_CODE_ERR_NOTNULL = 0, // boş değil
    EFS_CODE_ERR_FILENOTFOUND, // dosya bulunamadı
    EFS_CODE_ERR_INVALIDOPENTYPE, // geçersiz dosya açma türü
    EFS_CODE_ERR_OPENFAIL, // dosya açma hatası
    EFS_CODE_ERR_CLOSEFAIL, // dosya kapatma hatası
    EFS_CODE_ERR_PATHNULL, // dosya yolu boş
    EFS_CODE_ERR_NEWOBJECT, // yeni obje oluşturulamama hatası
    EFS_CODE_ERR_UNWRITTEN, // veri yazılamadı hatası

    // Argüman Değerler
    EFS_CODE_VAL_NOTCLOSE = 1000, // dosyayı kapatma
    EFS_CODE_VAL_CLOSE, // dosyayı kapat

    // Başarılı Durum Mesajlar
    EFS_CODE_MSG_OPENED = 2000, // dosya başarıyla açıldı
    EFS_CODE_MSG_CLOSED, // dosya başarıyla kapatıldı
    EFS_CODE_MSG_DATAWRITTEN, // veri başarıyla yazıldı

    // Durumlar
    EFS_CODE_STAT_FREE = 3000, // boşta
    EFS_CODE_STAT_ACTIVE, // aktif
    EFS_CODE_STAT_UNKNOWN, // bilinmiyor
    EFS_CODE_STAT_OPENEXCP, // dosya açmada istisna
    EFS_CODE_STAT_CLOSEXCP // dosya kapamada istisna
} efscode;

typedef const efscode efscode_s;

// Dosya Yapısı
typedef struct myfile
{
    fileptr addr; // dosya bellek adresi
    string_a path; // dosya yolu
    efsotype otype; // dosya açma türü
} MyFile;

typedef MyFile *MyFilePtr; // işaretçi
typedef const MyFile MyFile_s; // basit korumalı
typedef const MyFile* MyFilePtr_a; // bellek adresinden değiştirilebilir
typedef const MyFile* const MyFilePtr_s; // hiçbir şekilde değiştirilemez

// Fonksiyon Prototipleri
extern efscode_s file_opener(MyFilePtr myfileptr, string_s filepath, efsotype_s opentype);
extern efscode_s file_closer(MyFilePtr myfileptr);
extern efscode_s file_status(MyFilePtr_s myfileptr);
extern efscode_s file_write(MyFilePtr_s myfileptr, string_s data);

#endif