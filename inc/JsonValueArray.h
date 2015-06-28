/**
 * @file   JsonValueArray.h
 * @brief  JSON�̒l(array)
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEARRAY_H_
#define ANALYSISDRIVE_JSONVALUEARRAY_H_

#include "JsonValue.h"
#include "Json.h"
#include <vector>

/** Analysis Drive */
namespace adlib
{
/** JSON�̒l(array) */
class JsonValueArray final : public JsonValue
{
private:
  //! �l
  std::vector<Json> m_value;

private:
  /** �R�s�[�֎~ */
  JsonValueArray(const JsonValueArray&);

public:
  /** �R���X�g���N�^ */
  JsonValueArray() {}

  /**
   * �R���X�g���N�^
   * @tparam value �l
   */
  explicit JsonValueArray(const std::vector<Json>& value) : m_value(value) {}

  /** ���z�f�X�g���N�^ */
  virtual ~JsonValueArray() {}

  /**
   * ���
   * @param[in] obj ����I�u�W�F�N�g
   */
  JsonValueArray& operator=(const JsonValueArray& obj)
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
    return Json::JSON_VALUE_TYPE::ARRAY;
  }

  /**
   * array�l��get
   * @return array�l
   */
  std::vector<Json>& GetArray() override
  {
    return m_value;
  }

  /**
   * array�l��get(const��)
   * @return array�l
   */
  const std::vector<Json>& GetArray() const override
  {
    return m_value;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEARRAY_H_
