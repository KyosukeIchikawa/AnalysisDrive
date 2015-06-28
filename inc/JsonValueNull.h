/**
 * @file   JsonValueNull.h
 * @brief  JSON�̒l(null)
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUENULL_H_
#define ANALYSISDRIVE_JSONVALUENULL_H_

#include "JsonValue.h"
#include "Json.h"

/** Analysis Drive */
namespace adlib
{
/** JSON�̒l(bool) */
class JsonValueNull final : public JsonValue
{
private:
  /** �f�t�H���g�R���X�g���N�^�֎~ */
  JsonValueNull() {}
  /** �R�s�[�֎~ */
  JsonValueNull(const JsonValueNull&);
  /** ����֎~ */
  void operator=(const JsonValueNull&);

public:
  /**
   * �R���X�g���N�^
   * @tparam value nullptr
   */
  explicit JsonValueNull(std::nullptr_t value) {}

  /** ���z�f�X�g���N�^ */
  virtual ~JsonValueNull() {}

  /**
   * �^���擾
   * @return �^
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::NUL;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUENULL_H_
