// Slavnem @2024-07-07
// Downtimer File Stream
#pragma once

#ifndef DT_FILESTREAM_H
#define DT_FILESTREAM_H

// Standart Kütüphaneler
#include <stdio.h>

// DT Kütüphaneler
#include "dt_typedef.h"
#include "dt_macro.h"
#include "dt_allocation.h"

// Dosya türü isimlendirme
typedef FILE dtfs_file, *dtfs_fileptr;
typedef const dtfs_fileptr const dtfs_fileptr_s;

// DTFS Dosya Açma Türleri
// r = okur, dosya mevcut değilse hata verir
#define __DTFS_OTYPE_READ__ 10 // okuma
#define __DTFS_OTYPE_READWRITE__ 11 // okuma ve yazma
#define __DTFS_OTYPE_READBIN__ 12 // ikilik sistemde okuma
#define __DTFS_OTYPE_READWRITEBIN__ 13 // ikilik sistemde okuma ve yazma

// w = yazar, dosya yoksa oluşturur
#define __DTFS_OTYPE_WRITE__ 20 // yazma
#define __DTFS_OTYPE_WRITEREAD__ 21 // okuma ve yazma
#define __DTFS_OTYPE_WRITEBIN__ 22 // ikilik sistemde yazma
#define __DTFS_OTYPE_WRITEREADBIN__ 23 // ikilik sistemde okuma ve yazma

// a = dosya mevcutsa sona gder değilse oluşturur
#define __DTFS_OTYPE_ADD__ 30 // sondan eklemeye devam et
#define __DTFS_OTYPE_ADDREAD__ 31 // okuma ve ekleme
#define __DTFS_OTYPE_ADDBIN__ 32 // sondan eklemeye ikilik sistemde devam et, ekleme
#define __DTFS_OTYPE_ADDREADBIN__ 33 // sondan eklemeye ikilik sistemde devam et, okuma ve ekleme

// DTFS Dosya Uzantıları
#define __DTFS_LOG__ "dt.log" // kayıt tutmak için

// DTFS Kodları
typedef enum
{
    // DTFS Hataları
    DTFS_ERR_NOTNULL = 0, // boş değil
    DTFS_ERR_FILENOTFOUND, // dosya bulunamadı
    DTFS_ERR_INVALIDOPENTYPE, // geçersiz dosya açma türü
    DTFS_ERR_OPENFAIL, // dosya açma hatası
    DTFS_ERR_CLOSEFAIL, // dosya kapatma hatası
    DTFS_ERR_PATHNULL, // dosya yolu boş

    // DTFS Argüman Değerleri
    DTFS_VAL_NOTCLOSE = 1000, // dosyayı kapatma
    DTFS_VAL_CLOSE, // dosyayı kapat

    // DTFS Başarılı Durum Mesajları
    DTFS_MSG_OPENED = 2000, // dosya başarıyla açıldı
    DTFS_MSG_CLOSED, // dosya başarıyla kapatıldı

    // DTFS Durumları
    DTFS_STAT_FREE = 3000, // boşta
    DTFS_STAT_ACTIVE, // aktif
    DTFS_STAT_UNKNOWN, // bilinmiyor
    DTFS_STAT_OPENEXCP, // dosya açmada istisna
    DTFS_STAT_CLOSEXCP // dosya kapamada istisna
} dtfse_code;

// MY DTFS (Benim DT Dosya Sistemim) Yapısı
typedef struct
{
    dtfs_fileptr file_addr; // dosya adresi
    string_a file_path; // dosya yolu
    uint8_t open_type; // dosya açma türü
} mydtfs;

typedef mydtfs* mydtfsptr;
typedef const mydtfsptr const mydtfsptr_s;

//////////////////////////////////
// Fonksiyon Prototipleri
//////////////////////////////////
extern dtfse_code dtfsf_fileopener(mydtfsptr argMydtfsptr);
extern dtfse_code dtfsf_filecloser(mydtfsptr argMydtfsptr);
extern dtfse_code dtfsf_filestatus(mydtfsptr_s argMywdtfsptr);

//////////////////////////////////
// Fonksiyonlar
//////////////////////////////////
// Dosya Açıcı
/*
    Oluşturulan dosya yapısını kullanarak gerekli dosyayı açacak
    ve dosya başarıyla açılırsa değerler kaydedilecek ve başarı
    kodu döndürülecek, aksi halde hata kodu döndürülecek
*/
extern dtfse_code dtfsf_fileopener(mydtfsptr argMydtfsptr)
{
    // dosya açma türü
    string_a opentype = NULL;

    // dosya boş olmak zorunda, dosya adı boş olmamak zorunda
    // ve dosya türü geçerli bir tür olmak zorunda
    // sırasıyla bunları kontrol edeceğiz fakat ilk olarak dosya
    // kontrol edilecek, boş değilse hata döndürsün
    if(!isNull(argMydtfsptr->file_addr))
        return DTFS_ERR_NOTNULL; // dosya işaretçisi boş değil
    // dosya yolu kontrolü
    else if(isNull(argMydtfsptr->file_path))
        return DTFS_ERR_PATHNULL; // dosya yolu boş

    // dosya açma türleri kontrolü
    switch(argMydtfsptr->open_type)
    {
        case __DTFS_OTYPE_READ__: // okuma
            opentype = "r";
            break;
        case __DTFS_OTYPE_READBIN__: // ikilik okuma
            opentype = "rb";
            break;
        case __DTFS_OTYPE_READWRITE__: // okuma yazma
            opentype = "r+";
            break;
        case __DTFS_OTYPE_READWRITEBIN__: // ikilik okuma yazma
            opentype = "rb+";
            break;
        case __DTFS_OTYPE_WRITE__: // yazma
            opentype = "w";
            break;
        case __DTFS_OTYPE_WRITEBIN__: // ikilik yazma
            opentype = "wb";
            break;
        case __DTFS_OTYPE_WRITEREAD__: // yazma okuma
            opentype = "w+";
            break;
        case __DTFS_OTYPE_WRITEREADBIN__: // ikilik yazma okuma
            opentype = "wb+";
            break;
        case __DTFS_OTYPE_ADD__: // sona ekle
            opentype = "a";
            break;
        case __DTFS_OTYPE_ADDBIN__: // ikilik sona ekle
            opentype = "ab";
            break;
        case __DTFS_OTYPE_ADDREAD__: // sona ekle oku
            opentype = "a+";
            break;
        case __DTFS_OTYPE_ADDREADBIN__: // ikilik sona ekle oku
            opentype = "ab+";
            break; // sorunsuz
        // dosya açma türü geçersiz
        default:
            return DTFS_ERR_INVALIDOPENTYPE;
    }

    // dosyayı aç
    argMydtfsptr->file_addr = fopen(
        argMydtfsptr->file_path, // dosya yolu
        opentype // dosya açma türü
    );

    // eğer dosya adresi boş ise dosya açılamamıştır
    // aksi halde dosya başarıyla açılmıştır
    return isNull(argMydtfsptr->file_addr) ?
        DTFS_ERR_OPENFAIL // açma hatası
        :
        DTFS_MSG_OPENED; // başarıyla açıldı
}

// Açık Dosya Kapatıcı
/*
    Oluşturulan dosya yapısını kullanarak gerekli dosya eğer açıksa
    dosyayı kapatacak ve dosyayı bellekten temizleyecek fakat eğer dosya
    zaten kapalıysa ya da kapanmıyorsa, oluşan duruma göre hata ya da
    durum kodu döndürecektir aksi halde zaten kapatma başarılı olmuştur
    ve bu yüzden başarı kodu döndürür
*/
extern dtfse_code dtfsf_filecloser(mydtfsptr argMydtfsptr)
{
    // eğer dosya kapalıysa zaten kapatacak bir şey yoktur
    // gerekli ayarlamalar yapılıp sonlandırılır
    if(isNull(argMydtfsptr->file_addr))
        return DTFS_STAT_FREE; // boş olduğunu bildirmek için

    // dosyayı kapat
    fclose(argMydtfsptr->file_addr);
    argMydtfsptr->file_addr = NULL;

    // dosya başarıyla kapatılmışsa eğer, dosya işaretçisi boş olur
    // yani NULL olur ve bu sayede dosyanın kapandığını anlarız
    // aksi halde dosya kapanmamıştır
    return (!isNull(argMydtfsptr->file_addr)) ?
        DTFS_ERR_CLOSEFAIL // kapatma hatası
        :
        DTFS_MSG_CLOSED; // başaryıla kapatıldı
}

// Dosya Durumu Kontrolcüsü
/*
    Dosya yapısın inceleyerek dosyanın açık ya da kapalı olmasını
    kontrol eder fakat dosya yapısındaki verilere müdahele edemez
*/
extern dtfse_code dtfsf_filestatus(mydtfsptr_s argMywdtfsptr)
{
    // dosya boşsa bış durumu, değilse aktif durumunu döndür
    return (isNull(argMywdtfsptr->file_addr)) ?
        DTFS_STAT_FREE // dosya boş
        :
        DTFS_STAT_ACTIVE; // dosya aktif
}

#endif

#ifndef DT_FILESTREAM_H
#error "DT_FILESTREAM_H!"
#endif