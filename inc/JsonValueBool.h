/**
 * @file   JsonValueBool.h
 * @brief  JSON�̒l(bool)
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEBOOL_H_
#define ANALYSISDRIVE_JSONVALUEBOOL_H_

#include "JsonValue.h"
#include "Json.h"

/** Analysis Drive */
namespace adlib
{
/** JSON�̒l(bool) */
class JsonValueBool final : public JsonValue
{
private:
  //! �l
  bool m_value;

private:
  /** �f�t�H���g�R���X�g���N�^�֎~ */
  JsonValueBool() {}
  /** �R�s�[�֎~ */
  JsonValueBool(const JsonValueBool&);
  /** ����֎~ */
  void operator=(const JsonValueBool&);

public:
  /**
   * �R���X�g���N�^
   * @tparam value �l
   */
  explicit JsonValueBool(bool value) : m_value(value) {}

  /** ���z�f�X�g���N�^ */
  virtual ~JsonValueBool() {}

  /**
   * �^���擾
   * @return �^
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::BOOL;
  }

  /**
   * bool�l��get
   * @return bool�l
   */
  bool GetBool() const override
  {
    return m_value;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEBOOL_H_
