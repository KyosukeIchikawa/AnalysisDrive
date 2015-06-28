/**
 * @file   JsonValue.cpp
 * @brief  JSON�̒l
 * @author kyo
 * @date   2015/05/05
 */
#include "JsonValue.h"
#include "JsonValueString.h"
#include "JsonValueInt.h"
#include "JsonValueDouble.h"
#include "JsonValueBool.h"
#include "JsonValueArray.h"
#include "JsonValueObject.h"
#include <string>
#include <vector>
#include <map>

/** Analysis Drive */
namespace adlib
{
// �O���錾
class Json;

/**
 * �f�t�H���gstring�l��get
 * @return �󕶎�
 * @note �V���O���g���ɂ��邽��, JsonValue����͐؂藣��, �֐����ɐÓI�ȃf�t�H���g�l������
 */
static const std::string& DefaultString()
{
  static const JsonValueString defaultString("");
  return defaultString.GetString();
}

/**
 * �f�t�H���gnumber�l(int)��get
 * @return 0
 * @note �V���O���g���ɂ��邽��, JsonValue����͐؂藣��, �֐����ɐÓI�ȃf�t�H���g�l������
 */
static int DefaultInt()
{
  static const JsonValueInt defaultInt(0);
  return defaultInt.GetNumberInt();
}

/**
 * �f�t�H���gnumber�l(double)��get
 * @return 0
 * @note �V���O���g���ɂ��邽��, JsonValue����͐؂藣��, �֐����ɐÓI�ȃf�t�H���g�l������
 */
static double DefaultDouble()
{
  static const JsonValueDouble defaultDouble(0.0);
  return defaultDouble.GetNumberDouble();
}

/**
 * �f�t�H���gbool�l��get
 * @return false
 * @note �V���O���g���ɂ��邽��, JsonValue����͐؂藣��, �֐����ɐÓI�ȃf�t�H���g�l������
 */
static bool DefaultBool()
{
  static const JsonValueBool defaultBool(false);
  return defaultBool.GetBool();
}

/**
 * �f�t�H���garray�l��get
 * @return ��vector
 * @note �V���O���g���ɂ��邽��, JsonValue����͐؂藣��, �֐����ɐÓI�ȃf�t�H���g�l������
 */
static std::vector<Json>& DefaultArray()
{
  static JsonValueArray defaultArray;
  // ���������Ă���Ԃ�(�����ς���Ă���ꍇ�����邽��)
  defaultArray = JsonValueArray();
  return defaultArray.GetArray();
}

/**
 * �f�t�H���garray�l��get(const��)
 * @return ��vector
 * @note �V���O���g���ɂ��邽��, JsonValue����͐؂藣��, �֐����ɐÓI�ȃf�t�H���g�l������
 */
static const std::vector<Json>& DefaultConstArray()
{
  static const JsonValueArray defaultArray;
  return defaultArray.GetArray();
}

/**
 * �f�t�H���gobject�l��get
 * @return ��map
 * @note �V���O���g���ɂ��邽��, JsonValue����͐؂藣��, �֐����ɐÓI�ȃf�t�H���g�l������
 */
static std::map<std::string, Json>& DefaultObject()
{
  static JsonValueObject defaultObject;
  // ���������Ă���Ԃ�(�����ς���Ă���ꍇ�����邽��)
  defaultObject = JsonValueObject();
  return defaultObject.GetObject();
}

/**
 * �f�t�H���gobject�l��get(const��)
 * @return ��map
 * @note �V���O���g���ɂ��邽��, JsonValue����͐؂藣��, �֐����ɐÓI�ȃf�t�H���g�l������
 */
static const std::map<std::string, Json>& DefaultConstObject()
{
  static const JsonValueObject defaultObject;
  return defaultObject.GetObject();
}

/**
 * string�l��get
 * @return string�l
 * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
 *          �󕶎�""��Ԃ�
 */
const std::string& JsonValue::GetString() const
{
  return DefaultString();
}

/**
 * number�l(int)��get
 * @return number�l(int)
 * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
 *          0��Ԃ�
 */
int JsonValue::GetNumberInt() const
{
  return DefaultInt();
}

/**
 * number�l(double)��get
 * @return number�l(double)
 * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
 *          0.0��Ԃ�
 */
double JsonValue::GetNumberDouble() const
{
  return DefaultDouble();
}

/**
 * bool�l��get
 * @return bool�l
 * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
 *          false��Ԃ�
 */
bool JsonValue::GetBool() const
{
  return DefaultBool();
}

/**
 * array�l��get
 * @return array�l
 * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
 *          ���vector��Ԃ�
 */
std::vector<Json>& JsonValue::GetArray()
{
  return DefaultArray();
}

/**
 * array�l��get(const��)
 * @return array�l
 * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
 *          ���vector��Ԃ�
 */
const std::vector<Json>& JsonValue::GetArray() const
{
  return DefaultConstArray();
}

/**
 * object�l��get
 * @return object�l
 * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
 *          ���map��Ԃ�
 */
std::map<std::string, Json>& JsonValue::GetObject()
{
  return DefaultObject();
}

/**
 * object�l��get(const��)
 * @return object�l
 * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
 *          ���map��Ԃ�
 */
const std::map<std::string, Json>& JsonValue::GetObject() const
{
  return DefaultConstObject();
}
}
