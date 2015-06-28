/**
 * @file   JsonValueInt.h
 * @brief  JSON�̒l(number(int))
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEINT_H_
#define ANALYSISDRIVE_JSONVALUEINT_H_

#include "JsonValue.h"
#include "Json.h"
#include "IncludeMsgpack.h"

/** Analysis Drive */
namespace adlib
{
/** JSON�̒l(number(int)) */
class JsonValueInt final : public JsonValue
{
private:
  //! �l
  int m_value;

private:
  /** �f�t�H���g�R���X�g���N�^�֎~ */
  JsonValueInt() {}
  /** �R�s�[�֎~ */
  JsonValueInt(const JsonValueInt&);
  /** ����֎~ */
  void operator=(const JsonValueInt&);

public:
  /**
   * �R���X�g���N�^
   * @tparam value �l
   */
  explicit JsonValueInt(int value) : m_value(value) {}

  /** ���z�f�X�g���N�^ */
  virtual ~JsonValueInt() {}

  /**
   * �^���擾
   * @return �^
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::NUMBER_INT;
  }

  /**
   * number�l(int)��get
   * @return number�l(int)
   */
  int GetNumberInt() const override
  {
    return m_value;
  }

  /**
   * number�l(double)��get
   * @return number�l(double)
   */
  double GetNumberDouble() const override
  {
    return static_cast<double>(m_value);
  }

  /**
   * JSON�`���̕�������o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void Dump(std::ostream* outStream) const override
  {
    *outStream << m_value << std::flush;
  }

  /**
   * MessagePack�`���̃o�C�i�����o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void DumpMsgpack(std::ostream* outStream) const override
  {
    msgpack::pack(outStream, m_value);
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEINT_H_
