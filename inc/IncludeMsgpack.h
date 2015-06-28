/**
 * @file   IncludeMsgpack.h
 * @brief  MessagePackのinclude
 * @author kyo
 * @date   2015/06/27
 */
#ifndef ANALYSISDRIVE_INCLUDEMSGPACK_H_
#define ANALYSISDRIVE_INCLUDEMSGPACK_H_

#include <msgpack.hpp> // MessagePack

//----- MessgePackで定義されている不要なマクロを除去する -----//
#undef GetObject
#undef TRUE
#undef FALSE

#endif // ANALYSISDRIVE_INCLUDEMSGPACK_H_
