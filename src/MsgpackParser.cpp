/**
 * @file   MsgpackParser.cpp
 * @brief  MessagePack�p�[�T�[
 * @author kyo
 * @date   2015/06/29
 */
#include "MsgpackParser.h"
#include "Json.h"
#include "IncludeMsgpack.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

/** Analysis Drive */
namespace adlib
{
//> Pimpl�C�f�B�I��
class MsgpackParser::Impl
{
public:
  //> �G���[���b�Z�[�W���i�[����o�b�t�@�T�C�Y
  static const size_t BUFFER_SIZE;
  //> �G���[���b�Z�[�W --- �\�����ʃG���[
  static const std::string ERR_MSG_UNKNOWN;
  //> ���[�j���O���b�Z�[�W --- map��name���d�����Ă���
  static const std::string WRN_MSG_NAME_REPEATED;
  //> ���[�j���O���b�Z�[�W --- ���Ή��̌^(Binary�^)�����o
  static const std::string WRN_MSG_BINARY_TYPE_EXIST;
  //> ���[�j���O���b�Z�[�W --- ���Ή��̌^(Extension�^)�����o
  static const std::string WRN_MSG_EXTENSION_TYPE_EXIST;
  //> ���[�j���O���b�Z�[�W --- ���Ή��̌^�����o
  static const std::string WRN_MSG_UNKNOWN_TYPE_EXIST;

  //> �G���[���b�Z�[�W�R���e�i
  std::vector<std::string> m_errorMsg;
  //> ���[�j���O���b�Z�[�W�R���e�i
  std::vector<std::string> m_warningMsg;
  //> Map��name�d���t���O
  bool m_nameRepeated;

  /**
   * �R���X�g���N�^
   */
  Impl()
    :m_nameRepeated(false)
  {
  }

  /**
   * �G���[���b�Z�[�W���쐬����
   * @param[in] messageTemp �G���[���b�Z�[�W
   * @param[in] keyWord     ���b�Z�[�W�ɖ��ߍ��ރL�[���[�h
   * @return �G���[���b�Z�[�W
   * @remarks messageTemp�́u%s�v�̉ӏ���keyWord�𖄂ߍ���ŕԂ�
   */
  const std::string MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const;

  /**
   * string�^�Ƀp�[�X����
   * @param[in] msgobj MessagePack�I�u�W�F�N�g
   */
  Json ParseString(const msgpack::object& msgobj);

  /**
   * number�^(����)�Ƀp�[�X����
   * @param[in] msgobj MessagePack�I�u�W�F�N�g
   */
  Json ParseNumberInt(const msgpack::object& msgobj);

  /**
   * number�^(���������_��)�Ƀp�[�X����
   * @param[in] msgobj MessagePack�I�u�W�F�N�g
   */
  Json ParseNumberDouble(const msgpack::object& msgobj);

  /**
   * object�^�Ƀp�[�X����
   * @param[in] msgobj MessagePack�I�u�W�F�N�g
   */
  Json ParseObject(const msgpack::object& msgobj);

  /**
   * array�^�Ƀp�[�X����
   * @param[in] msgobj MessagePack�I�u�W�F�N�g
   */
  Json ParseArray(const msgpack::object& msgobj);

  /**
   * value�Ƀp�[�X����
   * @param[in] msgobj MessagePack�I�u�W�F�N�g
   */
  Json ParseValue(const msgpack::object& msgobj);
};

//> �G���[���b�Z�[�W���i�[����o�b�t�@�T�C�Y
const size_t MsgpackParser::Impl::BUFFER_SIZE = 256;
//> �G���[���b�Z�[�W --- �\�����ʃG���[
const std::string MsgpackParser::Impl::ERR_MSG_UNKNOWN("Unknown Error.");
//> ���[�j���O���b�Z�[�W --- map��name���d�����Ă���
const std::string MsgpackParser::Impl::WRN_MSG_NAME_REPEATED("Name \"%s\" Repeated, and Later Name is Given Priority.");
//> ���[�j���O���b�Z�[�W --- ���Ή��̌^(Binary�^)�����o
const std::string MsgpackParser::Impl::WRN_MSG_BINARY_TYPE_EXIST("Binary Type is not Supported, and null is Given Value.");
//> ���[�j���O���b�Z�[�W --- ���Ή��̌^(Extension�^)�����o
const std::string MsgpackParser::Impl::WRN_MSG_EXTENSION_TYPE_EXIST("Extension Type is not Supported, and null is Given Value.");
//> ���[�j���O���b�Z�[�W --- ���Ή��̌^�����o
const std::string MsgpackParser::Impl::WRN_MSG_UNKNOWN_TYPE_EXIST("Unknown Type is Detected, and null is Given Value.");

/**
 * �G���[���b�Z�[�W���쐬����
 * @param[in] messageTemp �G���[���b�Z�[�W
 * @param[in] keyWord     ���b�Z�[�W�ɖ��ߍ��ރL�[���[�h
 * @return �G���[���b�Z�[�W
 * @remarks messageTemp�́u%s�v�̉ӏ���keyWord�𖄂ߍ���ŕԂ�
 */
const std::string MsgpackParser::Impl::MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const
{
  // �o�b�t�@��p��
  char buf[Impl::BUFFER_SIZE];
  // �G���[���b�Z�[�W���쐬���ĕԂ�
  sprintf_s(buf, sizeof(buf), messageTemp.c_str(), keyWord.c_str());
  return std::string(buf);
}

/**
 * string�^�Ƀp�[�X����
 * @param[in] msgobj MessagePack�I�u�W�F�N�g
 */
Json MsgpackParser::Impl::ParseString(const msgpack::object& msgobj)
{
  // string�^�ɃR���o�[�g����
  std::string str;
  msgobj.convert(&str);
  // string�^Json�I�u�W�F�N�g��Ԃ�
  return Json(str);
}

/**
 * number�^(����)�Ƀp�[�X����
 * @param[in] msgobj MessagePack�I�u�W�F�N�g
 */
Json MsgpackParser::Impl::ParseNumberInt(const msgpack::object& msgobj)
{
  // int�^�ɃR���o�[�g����
  int num;
  msgobj.convert(&num);
  // int�^Json�I�u�W�F�N�g��Ԃ�
  return Json(num);
}

/**
 * number�^(���������_��)�Ƀp�[�X����
 * @param[in] msgobj MessagePack�I�u�W�F�N�g
 */
Json MsgpackParser::Impl::ParseNumberDouble(const msgpack::object& msgobj)
{
  // double�^�ɃR���o�[�g����
  double num;
  msgobj.convert(&num);
  // double�^Json�I�u�W�F�N�g��Ԃ�
  return Json(num);
}

/**
 * object�^�Ƀp�[�X����
 * @param[in] msgobj MessagePack�I�u�W�F�N�g
 */
Json MsgpackParser::Impl::ParseObject(const msgpack::object& msgobj)
{
  // �o�b�t�@��p��
  std::map<std::string, Json> obj;

  // �v�f������Ίi�[����
  if (msgobj.via.map.size > 0U)
  {
    //
    // ���L�̏����͖{��MessagePack�̃R�[�hobject.c���Q�l�ɂ���
    //

    // object_kv�\���̂��쐬
    msgpack::object_kv* p = msgobj.via.map.ptr;
    // �I�[��ݒ�
    msgpack::object_kv* const pend = msgobj.via.map.ptr + msgobj.via.map.size;
    // �I�[�܂Ń��[�v(�v�f�����[�v)
    for (; p < pend; ++p)
    {
      // �L�[���擾
      std::string key;
      p->key.convert(&key);

      // �L�[���d�����Ă���ꍇ
      if (obj.count(key) > 0)
      {
        // �d���t���O�����Ă�
        m_nameRepeated = true;
        // ���[�j���O���b�Z�[�W�i�[
        m_warningMsg.emplace_back(
          MakeErrorMsg(Impl::WRN_MSG_NAME_REPEATED, key));
      }

      // �l���i�[
      obj[key] = this->ParseValue(p->val);
    }
  }
  // object�^Json�I�u�W�F�N�g��Ԃ�
  return Json(obj);
}

/**
 * array�^�Ƀp�[�X����
 * @param[in] msgobj MessagePack�I�u�W�F�N�g
 */
Json MsgpackParser::Impl::ParseArray(const msgpack::object& msgobj)
{
  // �o�b�t�@��p��
  std::vector<Json> arr;

  // �v�f������Ίi�[����
  if (msgobj.via.array.size > 0U)
  {
    //
    // ���L�̏����͖{��MessagePack�̃R�[�hobject.c���Q�l�ɂ���
    //

    // msgpack_object�\���̂��쐬
    msgpack::object* p = msgobj.via.array.ptr;
    // �I�[��ݒ�
    msgpack::object* const pend = msgobj.via.array.ptr + msgobj.via.array.size;
    // �I�[�܂Ń��[�v(�v�f�����[�v)
    for (; p < pend; ++p)
    {
      // �l���i�[
      arr.emplace_back(this->ParseValue(*p));
    }
  }
  // array�^Json�I�u�W�F�N�g��Ԃ�
  return Json(arr);
}

/**
 * value�Ƀp�[�X����
 * @param[in] msgobj MessagePack�I�u�W�F�N�g
 */
Json MsgpackParser::Impl::ParseValue(const msgpack::object& msgobj)
{
  // object�^�C�v����
  switch (msgobj.type)
  {
    case msgpack::type::object_type::BOOLEAN:
      return Json(msgobj.via.boolean);

    case msgpack::type::object_type::FLOAT:
      return this->ParseNumberDouble(msgobj);

    case msgpack::type::object_type::POSITIVE_INTEGER:
    case msgpack::type::object_type::NEGATIVE_INTEGER:
      return this->ParseNumberInt(msgobj);

    case msgpack::type::object_type::STR:
      return this->ParseString(msgobj);

    case msgpack::type::object_type::NIL:
      return Json(nullptr);

    case msgpack::type::object_type::ARRAY:
      return this->ParseArray(msgobj);

    case msgpack::type::object_type::MAP:
      return this->ParseObject(msgobj);

    case msgpack::type::object_type::BIN:
      // Binary�^�͖��Ή��Ȃ̂�, ���[�j���O���b�Z�[�W�i�[
      m_warningMsg.emplace_back(Impl::WRN_MSG_BINARY_TYPE_EXIST);
      // �ꉞnull�Ƃ��Ă���
      return Json(nullptr);

    case msgpack::type::object_type::EXT:
      // Extension�^�͖��Ή��Ȃ̂�, ���[�j���O���b�Z�[�W�i�[
      m_warningMsg.emplace_back(Impl::WRN_MSG_EXTENSION_TYPE_EXIST);
      // �ꉞnull�Ƃ��Ă���
      return Json(nullptr);

    default:
      // ���Ή��̌^�Ȃ̂�, ���[�j���O���b�Z�[�W�i�[
      m_warningMsg.emplace_back(Impl::WRN_MSG_UNKNOWN_TYPE_EXIST);
      // �ꉞnull�Ƃ��Ă���
      return Json(nullptr);
  }
}

/** �R���X�g���N�^ */
MsgpackParser::MsgpackParser()
  : m_impl(std::make_unique<Impl>())
{
}

/** �f�X�g���N�^ */
MsgpackParser::~MsgpackParser() {}

/**
 * MessagePack�`���̕�����Ƀp�[�X����
 * @param[in] target �p�[�X�Ώۂ̕�����
 * @return Json�I�u�W�F�N�g
 * @attention ����܂ł̃G���[���b�Z�[�W�⃏�[�j���O���b�Z�[�W�������������
 */
Json MsgpackParser::Parse(const std::string& target)
{
  // �O��̃p�[�X���ʂ�������
  m_impl = std::make_unique<Impl>();

  // �f�V���A���C�Y
  msgpack::unpacked result;
  msgpack::unpack(result, target.data(), target.size());
  const msgpack::object& msgobj = result.get();

  try
  {
    // �p�[�X����
    return m_impl->ParseValue(msgobj);
  }
  catch (const std::string& err)
  {
    // �G���[���b�Z�[�W�i�[
    m_impl->m_errorMsg.emplace_back(err);
    // ���s����null�^Json�I�u�W�F�N�g��Ԃ�
    return Json(nullptr);
  }
  catch (...) // �\�����ʃG���[(MessagePack�֘A�̃G���[�Ȃ�)
  {
    // �G���[���b�Z�[�W�i�[
    m_impl->m_errorMsg.emplace_back(Impl::ERR_MSG_UNKNOWN);
    // ���s����null�^Json�I�u�W�F�N�g��Ԃ�
    return Json(nullptr);
  }
}

/**
 * �G���[���b�Z�[�W���擾����
 * @return �G���[���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
 */
const std::vector<std::string>& MsgpackParser::ErrorMessage() const
{
  return m_impl->m_errorMsg;
}

/**
 * ���[�j���O���b�Z�[�W���擾����
 * @return ���[�j���O���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
 */
const std::vector<std::string>& MsgpackParser::WarningMessage() const
{
  return m_impl->m_warningMsg;
}

/**
 * Map��name���d�����Ă����true��Ԃ�
 * @return �O���Parse��Map��name���d�����Ă����true��Ԃ�
 */
bool MsgpackParser::IsNameRepeated() const
{
  return m_impl->m_nameRepeated;
}
}
