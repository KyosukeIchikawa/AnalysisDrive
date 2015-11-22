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
#include "IncludeMsgpack.h"
#include <string>
#include <unordered_map>

/** Analysis Drive */
namespace adlib
{
/** JSON�̒l(object) */
class JsonValueObject final : public JsonValue
{
private:
  //! �l
  Json::Object m_value;

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
  explicit JsonValueObject(const Json::Object& value) : m_value(value) {}

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
  Json::Object& GetObject() override
  {
    return m_value;
  }

  /**
   * object�l��get(const��)
   * @return object�l
   */
  const Json::Object& GetObject() const override
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
    *outStream << "{";
    // �v�f������ꍇ�̂ݏo�͏������s��
    if (m_value.size() > 0)
    {
      // �C�e���[�^�擾
      auto it = m_value.begin();
      // �v�f�����[�v
      while (true)
      {
        // name���o��
        *outStream << "\"" << it->first << "\"" << ":";
        // �o��
        it->second.Dump(outStream);
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
    *outStream << "}" << std::flush;
  }

  /**
   * MessagePack�`���̃o�C�i�����o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void DumpMsgpack(std::ostream* outStream) const override
  {
    msgpack::packer<std::ostream> pk(outStream);
    // object�T�C�Y���w��
    pk.pack_map(m_value.size());
    // �v�f���o��
    for (auto it = m_value.begin(); it != m_value.end(); ++it)
    {
      // name���o��
      pk.pack(it->first);
      // value���o��
      it->second.DumpMsgpack(outStream);
    }
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEOBJECT_H_
