/**
 * @file   JsonValueString.h
 * @brief  JSON�̒l(string)
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUESTRING_H_
#define ANALYSISDRIVE_JSONVALUESTRING_H_

#include "JsonValue.h"
#include "Json.h"
#include <string>

/** Analysis Drive */
namespace adlib
{
/** JSON�̒l(string) */
class JsonValueString final : public JsonValue
{
private:
  //! �l
  const std::string m_value;

private:
  /** �f�t�H���g�R���X�g���N�^�֎~ */
  JsonValueString() {}
  /** �R�s�[�֎~ */
  JsonValueString(const JsonValueString&);
  /** ����֎~ */
  void operator=(const JsonValueString&);

public:
  /**
   * �R���X�g���N�^
   * @tparam value �l
   */
  explicit JsonValueString(const std::string& value) : m_value(value) {}

  /** ���z�f�X�g���N�^ */
  virtual ~JsonValueString() {}

  /**
   * �^���擾
   * @return �^
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::STRING;
  }

  /**
   * string�l��get
   * @return string�l
   */
  const std::string& GetString() const override
  {
    return m_value;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUESTRING_H_
