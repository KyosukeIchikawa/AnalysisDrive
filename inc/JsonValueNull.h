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
#include "IncludeMsgpack.h"

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

  /**
   * JSON�`���̕�������o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void Dump(std::ostream* outStream) const override
  {
    *outStream << "null" << std::flush;
  }

  /**
   * MessagePack�`���̃o�C�i�����o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void DumpMsgpack(std::ostream* outStream) const override
  {
    msgpack::pack(outStream, msgpack::type::nil());
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUENULL_H_
