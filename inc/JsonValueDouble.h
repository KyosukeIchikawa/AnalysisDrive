/**
 * @file   JsonValueDouble.h
 * @brief  JSON�̒l(number(double))
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEDOUBLE_H_
#define ANALYSISDRIVE_JSONVALUEDOUBLE_H_

#include "JsonValue.h"
#include "Json.h"

/** Analysis Drive */
namespace adlib
{
/** JSON�̒l(number(double)) */
class JsonValueDouble final : public JsonValue
{
private:
  //! �l
  double m_value;

private:
  /** �f�t�H���g�R���X�g���N�^�֎~ */
  JsonValueDouble() {}
  /** �R�s�[�֎~ */
  JsonValueDouble(const JsonValueDouble&);
  /** ����֎~ */
  void operator=(const JsonValueDouble&);

public:
  /**
   * �R���X�g���N�^
   * @tparam value �l
   */
  explicit JsonValueDouble(double value) : m_value(value) {}

  /** ���z�f�X�g���N�^ */
  virtual ~JsonValueDouble() {}

  /**
   * �^���擾
   * @return �^
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::NUMBER_DOUBLE;
  }

  /**
   * number�l(int)��get
   * @return number�l(int)
   */
  int GetNumberInt() const override
  {
    return static_cast<int>(m_value);
  }

  /**
   * number�l(double)��get
   * @return number�l(double)
   */
  double GetNumberDouble() const override
  {
    return m_value;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEDOUBLE_H_
