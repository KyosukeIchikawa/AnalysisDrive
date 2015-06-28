/**
 * @file   JsonValue.cpp
 * @brief  JSONの値
 * @author kyo
 * @date   2015/05/05
 */

//----- MessagePack -----//
/*
   Msgpack is Copyright (C) 2008-2014 FURUHASHI Sadayuki
   and licensed under the Apache License, Version 2.0, as follows.

   ------------------------------------------------------------------------
   Copyright (C) 2008-2010 FURUHASHI Sadayuki

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   ------------------------------------------------------------------------
 */
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "msgpackd.lib") // Debug版
#else
#pragma comment(lib, "msgpack.lib") // Release版
#endif

#include "JsonValue.h"
#include "JsonValueString.h"
#include "JsonValueInt.h"
#include "JsonValueDouble.h"
#include "JsonValueBool.h"
#include "JsonValueArray.h"
#include "JsonValueObject.h"
#include "IncludeMsgpack.h"
#include <string>
#include <vector>
#include <map>

/** Analysis Drive */
namespace adlib
{
// 前方宣言
class Json;

/**
 * デフォルトstring値のget
 * @return 空文字
 * @note シングルトンにするため, JsonValueからは切り離し, 関数内に静的なデフォルト値を持つ
 */
static const std::string& DefaultString()
{
  static const JsonValueString defaultString("");
  return defaultString.GetString();
}

/**
 * デフォルトnumber値(int)のget
 * @return 0
 * @note シングルトンにするため, JsonValueからは切り離し, 関数内に静的なデフォルト値を持つ
 */
static int DefaultInt()
{
  static const JsonValueInt defaultInt(0);
  return defaultInt.GetNumberInt();
}

/**
 * デフォルトnumber値(double)のget
 * @return 0
 * @note シングルトンにするため, JsonValueからは切り離し, 関数内に静的なデフォルト値を持つ
 */
static double DefaultDouble()
{
  static const JsonValueDouble defaultDouble(0.0);
  return defaultDouble.GetNumberDouble();
}

/**
 * デフォルトbool値のget
 * @return false
 * @note シングルトンにするため, JsonValueからは切り離し, 関数内に静的なデフォルト値を持つ
 */
static bool DefaultBool()
{
  static const JsonValueBool defaultBool(false);
  return defaultBool.GetBool();
}

/**
 * デフォルトarray値のget
 * @return 空vector
 * @note シングルトンにするため, JsonValueからは切り離し, 関数内に静的なデフォルト値を持つ
 */
static std::vector<Json>& DefaultArray()
{
  static JsonValueArray defaultArray;
  // 初期化してから返す(書き変わっている場合があるため)
  defaultArray = JsonValueArray();
  return defaultArray.GetArray();
}

/**
 * デフォルトarray値のget(const版)
 * @return 空vector
 * @note シングルトンにするため, JsonValueからは切り離し, 関数内に静的なデフォルト値を持つ
 */
static const std::vector<Json>& DefaultConstArray()
{
  static const JsonValueArray defaultArray;
  return defaultArray.GetArray();
}

/**
 * デフォルトobject値のget
 * @return 空map
 * @note シングルトンにするため, JsonValueからは切り離し, 関数内に静的なデフォルト値を持つ
 */
static std::map<std::string, Json>& DefaultObject()
{
  static JsonValueObject defaultObject;
  // 初期化してから返す(書き変わっている場合があるため)
  defaultObject = JsonValueObject();
  return defaultObject.GetObject();
}

/**
 * デフォルトobject値のget(const版)
 * @return 空map
 * @note シングルトンにするため, JsonValueからは切り離し, 関数内に静的なデフォルト値を持つ
 */
static const std::map<std::string, Json>& DefaultConstObject()
{
  static const JsonValueObject defaultObject;
  return defaultObject.GetObject();
}

/**
 * string値のget
 * @return string値
 * @remarks オーバーライドされない場合の動作 <br>
 *          空文字""を返す
 */
const std::string& JsonValue::GetString() const
{
  return DefaultString();
}

/**
 * number値(int)のget
 * @return number値(int)
 * @remarks オーバーライドされない場合の動作 <br>
 *          0を返す
 */
int JsonValue::GetNumberInt() const
{
  return DefaultInt();
}

/**
 * number値(double)のget
 * @return number値(double)
 * @remarks オーバーライドされない場合の動作 <br>
 *          0.0を返す
 */
double JsonValue::GetNumberDouble() const
{
  return DefaultDouble();
}

/**
 * bool値のget
 * @return bool値
 * @remarks オーバーライドされない場合の動作 <br>
 *          falseを返す
 */
bool JsonValue::GetBool() const
{
  return DefaultBool();
}

/**
 * array値のget
 * @return array値
 * @remarks オーバーライドされない場合の動作 <br>
 *          空のvectorを返す
 */
std::vector<Json>& JsonValue::GetArray()
{
  return DefaultArray();
}

/**
 * array値のget(const版)
 * @return array値
 * @remarks オーバーライドされない場合の動作 <br>
 *          空のvectorを返す
 */
const std::vector<Json>& JsonValue::GetArray() const
{
  return DefaultConstArray();
}

/**
 * object値のget
 * @return object値
 * @remarks オーバーライドされない場合の動作 <br>
 *          空のmapを返す
 */
std::map<std::string, Json>& JsonValue::GetObject()
{
  return DefaultObject();
}

/**
 * object値のget(const版)
 * @return object値
 * @remarks オーバーライドされない場合の動作 <br>
 *          空のmapを返す
 */
const std::map<std::string, Json>& JsonValue::GetObject() const
{
  return DefaultConstObject();
}

/**
 * JSON形式の文字列を出力
 * @param[in,out] outStream 出力先のストリーム
 */
void JsonValue::Dump(std::ostream* outStream) const
{
  // nullを出力
  *outStream << "null" << std::flush;
}

/**
 * MessagePack形式のバイナリを出力
 * @param[in,out] outStream 出力先のストリーム
 */
void JsonValue::DumpMsgpack(std::ostream* outStream) const
{
  msgpack::pack(outStream, msgpack::type::nil());
}
}
