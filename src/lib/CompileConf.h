// Slavnem @2024-07-28
// Derlemek İçin Ayar Saklamak
#ifndef DT_COMPILER_H
#define DT_COMPILER_H

// Son Kullanıcı Modu
// #define __COMPILE_RELEASE_MODE__

// Hata Ayıklama Modu
#ifndef __COMPILE_RELEASE_MODE__
    #define __COMPILE_DEBUG__MODE__
#endif

#endif