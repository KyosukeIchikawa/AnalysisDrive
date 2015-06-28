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
#include "IncludeMsgpack.h"
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

  /**
   * JSON�`���̕�������o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void Dump(std::ostream* outStream) const override
  {
    // �O����
    *outStream << "[";
    // �v�f������ꍇ�̂ݏo�͏������s��
    if (m_value.size() > 0)
    {
      // �C�e���[�^�擾
      auto it = m_value.begin();
      // �v�f�����[�v
      while (true)
      {
        // �o��
        it->Dump(outStream);
        // �C�e���[�^��i�߂�
        it++;
        // �I���łȂ��ꍇ
        if (it != m_value.end())
        {
          // �J���}���o��
          *outStream << ",";
        }
        // �I���̏ꍇ
        else
        {
          break;
        }
      }
    }
    // �㊇��
    *outStream << "]" << std::flush;
  }

  /**
   * MessagePack�`���̃o�C�i�����o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void DumpMsgpack(std::ostream* outStream) const override
  {
    msgpack::packer<std::ostream> pk(outStream);
    // array�T�C�Y���w��
    pk.pack_array(m_value.size());
    // �v�f���o��
    for (auto it = m_value.begin(); it != m_value.end(); ++it)
    {
      // �o��
      it->DumpMsgpack(outStream);
    }
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEARRAY_H_
