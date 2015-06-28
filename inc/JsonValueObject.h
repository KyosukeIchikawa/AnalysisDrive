/**
 * @file   JsonValueObject.h
 * @brief  JSON�̒l(object)
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEOBJECT_H_
#define ANALYSISDRIVE_JSONVALUEOBJECT_H_

#include "JsonValue.h"
#include "Json.h"
#include <string>
#include <map>

/** Analysis Drive */
namespace adlib
{
/** JSON�̒l(object) */
class JsonValueObject final : public JsonValue
{
private:
  //! �l
  std::map<std::string, Json> m_value;

private:
  /** �R�s�[�֎~ */
  JsonValueObject(const JsonValueObject&);

public:
  /** �R���X�g���N�^ */
  JsonValueObject() {}

  /**
   * �R���X�g���N�^
   * @tparam value �l
   */
  explicit JsonValueObject(const std::map<std::string, Json>& value) : m_value(value) {}

  /** ���z�f�X�g���N�^ */
  virtual ~JsonValueObject() {}

  /**
   * ���
   * @param[in] obj ����I�u�W�F�N�g
   */
  JsonValueObject& operator=(const JsonValueObject& obj)
  {
    m_value = obj.m_value;
    return *this;
  }

  /**
   * �^���擾
   * @return �^
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::OBJECT;
  }

  /**
   * object�l��get
   * @return object�l
   */
  std::map<std::string, Json>& GetObject() override
  {
    return m_value;
  }

  /**
   * object�l��get(const��)
   * @return object�l
   */
  const std::map<std::string, Json>& GetObject() const override
  {
    return m_value;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEOBJECT_H_
