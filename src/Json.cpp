/**
 * @file   Json.cpp
 * @brief  JSON�I�u�W�F�N�g
 * @author kyo
 * @date   2015/05/03
 */
#include "Json.h"
#include "JsonValue.h"
#include "JsonValueNull.h"
#include "JsonValueString.h"
#include "JsonValueInt.h"
#include "JsonValueDouble.h"
#include "JsonValueBool.h"
#include "JsonValueArray.h"
#include "JsonValueObject.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

/** Analysis Drive */
namespace adlib
{
//! Pimpl�C�f�B�I��
class Json::Impl
{
private:
  /* �f�t�H���g�R���X�g���N�^�֎~ */
  Impl();

public:
  //! JSON�̒l
  std::shared_ptr<JsonValue> m_jsonValue;

  /**
   * �R���X�g���N�^
   * @param[in] value �l(null)
   */
  explicit Impl(std::nullptr_t value)
    :m_jsonValue(std::make_shared<JsonValueNull>(value))
  {
  }

  /**
   * �R���X�g���N�^
   * @param[in] value �l(string�^)
   */
  explicit Impl(const std::string& value)
    :m_jsonValue(std::make_shared<JsonValueString>(value))
  {
  }

  /**
   * �R���X�g���N�^
   * @param[in] value �l(number�^(int))
   */
  explicit Impl(int value)
    :m_jsonValue(std::make_shared<JsonValueInt>(value))
  {
  }

  /**
   * �R���X�g���N�^
   * @param[in] value �l(number�^(double))
   */
  explicit Impl(double value)
    :m_jsonValue(std::make_shared<JsonValueDouble>(value))
  {
  }

  /**
   * �R���X�g���N�^
   * @param[in] value �l(bool�^)
   */
  explicit Impl(bool value)
    :m_jsonValue(std::make_shared<JsonValueBool>(value))
  {
  }

  /**
   * �R���X�g���N�^
   * @param[in] value �l(array�^)
   */
  explicit Impl(const std::vector<Json>& value)
    :m_jsonValue(std::make_shared<JsonValueArray>(value))
  {
  }

  /**
   * �R���X�g���N�^
   * @param[in] value �l(object�^)
   */
  explicit Impl(const std::map<std::string, Json>& value)
    :m_jsonValue(std::make_shared<JsonValueObject>(value))
  {
  }

  /**
   * �R�s�[�R���X�g���N�^
   * @param[in] obj �R�s�[��
   */
  Impl(const Impl& obj)
    :m_jsonValue(obj.m_jsonValue)
  {
  }

  /**
   * ���[�u�R���X�g���N�^
   * @param[in] obj ���[�u�I�u�W�F�N�g
   */
  Impl(Impl&& obj)
    :m_jsonValue(std::move(obj.m_jsonValue))
  {
  }

  /**
   * ���
   * @param[in] obj �������I�u�W�F�N�g
   */
  Impl& Impl::operator=(const Impl& obj)
  {
    m_jsonValue = obj.m_jsonValue;
    return *this;
  }
};

/**
 * �R���X�g���N�^
 * @remarks �����Ȃ��̃R���X�g���N�^�ł�null��JSON�I�u�W�F�N�g���쐬����
 * @note std::map<std::string, Json>�𑶍݂��Ȃ��L�[�ŃA�N�Z�X�����ꍇ, <br>
 *       �{�R���X�g���N�^�ɂ����Json���V�K�o�^�����. <br>
 *       ����ɂ��, ���݂��Ȃ��L�[�ŃA�N�Z�X�����null�Ɠ��l�̒l�ɂȂ�.
 */
Json::Json() :m_impl(std::make_unique<Impl>(nullptr)) {}

/**
 * �R���X�g���N�^
 * @param[in] value �l(null)
 */
Json::Json(std::nullptr_t value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * �R���X�g���N�^
 * @param[in] value �l(string�^)
 */
Json::Json(const std::string& value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * �R���X�g���N�^
 * @param[in] value �l(number�^(int))
 */
Json::Json(int value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * �R���X�g���N�^
 * @param[in] value �l(number�^(double))
 */
Json::Json(double value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * �R���X�g���N�^
 * @param[in] value �l(bool�^)
 */
Json::Json(bool value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * �R���X�g���N�^
 * @param[in] value �l(array�^)
 */
Json::Json(const std::vector<Json>& value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * �R���X�g���N�^
 * @param[in] value �l(object�^)
 */
Json::Json(const std::map<std::string, Json>& value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * �R�s�[�R���X�g���N�^
 * @param[in] obj �R�s�[��
 */
Json::Json(const Json& obj) :m_impl(std::make_unique<Impl>(*(obj.m_impl)))
{
}

/**
 * ���[�u�R���X�g���N�^
 * @param[in] obj ���[�u�I�u�W�F�N�g
 */
Json::Json(Json&& obj) :m_impl(std::move(obj.m_impl)) {}

/**
 * ���
 * @param[in] obj �������I�u�W�F�N�g
 */
Json& Json::operator=(const Json& obj)
{
  *(this->m_impl) = *(obj.m_impl);
  return *this;
}

/** ���z�f�X�g���N�^ */
Json::~Json() {}

/**
 * �^���擾
 * @return �^
 */
Json::JSON_VALUE_TYPE Json::GetValueType() const
{
  return m_impl->m_jsonValue->GetValueType();
}

/**
 * null���ǂ���
 * @return null�Ȃ�true
 */
bool Json::IsNull() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::NUL;
}

/**
 * string���ǂ���
 * @return string�Ȃ�true
 */
bool Json::IsString() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::STRING;
}

/**
 * number(int or double)���ǂ���
 * @return number�Ȃ�true
 */
bool Json::IsNumber() const
{
  Json::JSON_VALUE_TYPE type = m_impl->m_jsonValue->GetValueType();
  return (type == Json::JSON_VALUE_TYPE::NUMBER_INT) || (type == Json::JSON_VALUE_TYPE::NUMBER_DOUBLE);
}

/**
 * number(int)���ǂ���
 * @return number(int)�Ȃ�true
 */
bool Json::IsNumberInt() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::NUMBER_INT;
}

/**
 * number(double)���ǂ���
 * @return number(double)�Ȃ�true
 */
bool Json::IsNumberDouble() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::NUMBER_DOUBLE;
}

/**
 * bool���ǂ���
 * @return bool�Ȃ�true
 */
bool Json::IsBool() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::BOOL;
}

/**
 * array���ǂ���
 * @return array�Ȃ�true
 */
bool Json::IsArray() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::ARRAY;
}

/**
 * object���ǂ���
 * @return object�Ȃ�true
 */
bool Json::IsObject() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::OBJECT;
}

/**
 * string�l��get
 * @return string�l
 */
const std::string& Json::GetString() const
{
  return m_impl->m_jsonValue->GetString();
}

/**
 * number�l(int)��get
 * @return number�l(int)
 */
int Json::GetNumberInt() const
{
  return m_impl->m_jsonValue->GetNumberInt();
}

/**
 * number(double)��get
 * @return number�l(double)
 */
double Json::GetNumberDouble() const
{
  return m_impl->m_jsonValue->GetNumberDouble();
}

/**
 * bool�l��get
 * @return bool�l
 */
bool Json::GetBool() const
{
  return m_impl->m_jsonValue->GetBool();
}

/**
 * array�l��get
 * @return array�l
 */
std::vector<Json>& Json::GetArray()
{
  return m_impl->m_jsonValue->GetArray();
}

/**
 * array�l��get(const��)
 * @return array�l
 */
const std::vector<Json>& Json::GetArray() const
{
  return m_impl->m_jsonValue->GetArray();
}

/**
 * object�l��get
 * @return object�l
 */
std::map<std::string, Json>& Json::GetObject()
{
  return m_impl->m_jsonValue->GetObject();
}

/**
 * object�l��get(const��)
 * @return object�l
 */
const std::map<std::string, Json>& Json::GetObject() const
{
  return m_impl->m_jsonValue->GetObject();
}

/**
 * JSON�`���̕�������o��
 * @param[in,out] outStream �o�͐�̃X�g���[��
 */
void Json::Dump(std::ostream* outStream) const
{
  m_impl->m_jsonValue->Dump(outStream);
}

/**
 * MessagePack�`���̃o�C�i�����o��
 * @param[in,out] outStream �o�͐�̃X�g���[��
 */
void Json::DumpMsgpack(std::ostream* outStream) const
{
  m_impl->m_jsonValue->DumpMsgpack(outStream);
}
}
