/**
 * @file   JsonParser.cpp
 * @brief  JSON�p�[�T�[
 * @author kyo
 * @date   2015/05/07
 */
#include "JsonParser.h"
#include "Json.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

/** Analysis Drive */
namespace adlib
{
//> Pimpl�C�f�B�I��
class JsonParser::Impl
{
public:
  //> ���^�[���R�[�h
  enum RETURN_CODE
  {
    SUCCESS, /*!< ���� */
    FAILURE  /*!< ���s */
  };

  //> UNICODE�G�X�P�[�v�V�[�P���X��4BYTE�̈�
  static const size_t ESCAPE_SEQUENCE_4BYTE;
  //> 16�i���̈�
  static const int HEXADECIMAL_16;
  //> UTF8��BOM�T�C�Y
  static const int BOM_UTF8_SIZE;
  //> UTF8��BOM
  static const char BOM_UTF8[];
  //> �G���[���b�Z�[�W���i�[����o�b�t�@�T�C�Y
  static const size_t BUFFER_SIZE;
  //> �G���[���b�Z�[�W --- JSON�̃p�[�X���s
  static const std::string ERR_MSG_PARSE_FAILURE;
  //> �G���[���b�Z�[�W --- 0�ړ���number�^������
  static const std::string ERR_MSG_0HEAD_NUMBER;
  //> �G���[���b�Z�[�W --- number�^�ɖ����ȕ������܂܂�Ă���
  static const std::string ERR_MSG_BAD_WORD_IN_NUMBER;
  //> �G���[���b�Z�[�W --- number�^(int�^)���͈͊O�̒l
  static const std::string ERR_MSG_INT_NUMBER_OUT_OF_RANGE;
  //> �G���[���b�Z�[�W --- number�^(double�^)���͈͊O�̒l
  static const std::string ERR_MSG_DOUBLE_NUMBER_OUT_OF_RANGE;
  //> �G���[���b�Z�[�W --- '.'�̌㑱�ɐ��l������
  static const std::string ERR_MSG_NOT_DIGIT_FOLLOWER_DOT;
  //> �G���[���b�Z�[�W --- 'e', 'E'�̌㑱�ɐ��l������
  static const std::string ERR_MSG_NOT_DIGIT_FOLLOWER_EXP;
  //> �G���[���b�Z�[�W --- ����������'"'���Ȃ�
  static const std::string ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION;
  //> �G���[���b�Z�[�W --- �s���ȃG�X�P�[�v����������
  static const std::string ERR_MSG_ESCAPE_CHARACTER_FAILURE;
  //> �G���[���b�Z�[�W --- �T���Q�[�g�̈��UNICODE������
  static const std::string ERR_MSG_EXIST_CODE_IN_SURROGATE;
  //> �G���[���b�Z�[�W --- object�����'}'���Ȃ�
  static const std::string ERR_MSG_NOT_EXIST_BRACKET_IN_OBJ;
  //> �G���[���b�Z�[�W --- object����2�ڈȍ~�̒l�̑O��','���Ȃ�
  static const std::string ERR_MSG_NOT_EXIST_COMMA_IN_OBJ;
  //> �G���[���b�Z�[�W --- object�ɕ�����J�n��'"'���Ȃ�
  static const std::string ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION_IN_OBJ;
  //> �G���[���b�Z�[�W --- object��':'���Ȃ�
  static const std::string ERR_MSG_NOT_EXIST_COLON_IN_OBJ;
  //> �G���[���b�Z�[�W --- array�����']'���Ȃ�
  static const std::string ERR_MSG_NOT_EXIST_BRACKET_IN_ARRAY;
  //> �G���[���b�Z�[�W --- array����2�ڈȍ~�̒l�̑O��','���Ȃ�
  static const std::string ERR_MSG_NOT_EXIST_COMMA_IN_ARRAY;
  //> �G���[���b�Z�[�W --- �s���Ȓl������
  static const std::string ERR_MSG_VALUE_FAILURE;
  //> ���[�j���O���b�Z�[�W --- object��name���d�����Ă���
  static const std::string WRN_MSG_NAME_REPEATED;

  //> �G���[���b�Z�[�W�R���e�i
  std::vector<std::string> m_errorMsg;
  //> ���[�j���O���b�Z�[�W�R���e�i
  std::vector<std::string> m_warningMsg;
  //> object��name�d���t���O
  bool m_nameRepeated;
  //> �p�[�X�Ώۂ̕�����
  const std::string& m_target;
  //> �p�[�X�Ώۂ̕�����̃J�[�\��(���ɓǂݍ������Ƃ���ʒu)
  size_t m_i;

  /**
   * �R���X�g���N�^
   * @param[in] target �p�[�X�Ώۂ̕�����
   */
  Impl(const std::string& target)
    :m_nameRepeated(false),
     m_target(target),
     m_i(0)
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
   * ����1�������擾(�z���C�g�X�y�[�X�̓X�L�b�v)
   * @param[out] word �i�[��
   * @return ���^�[���R�[�h(�擾��������SUCCESS)
   * @remarks �{�֐��I����, �J�[�\���͎���1�������w��(�z���C�g�X�y�[�X�̓X�L�b�v)
   */
  Impl::RETURN_CODE SkipWhitespaceAndGetWord(char* word);

  /**
   * UTF8��BOM������΃X�L�b�v����
   */
  void SkipBOM();

  /**
   * �G�X�P�[�v�V�[�P���X�𕶎���ɕϊ�����
   * @param[in] hex UNICODE16�i��
   */
  const std::string ConversionEscapeSequence(int hex);

  /**
   * string�^���p�[�X����
   * @remarks �{�֐��I����, �J�[�\����string�^�̎���1�������w��
   */
  Json ParseString();

  /**
   * number�^���p�[�X����
   * @remarks �{�֐��I����, �J�[�\����number�^�̎���1�������w��
   */
  Json ParseNumber();

  /**
   * object�^���p�[�X����
   * @remarks �{�֐��I����, �J�[�\����object�^�̎���1�������w��
   */
  Json ParseObject();

  /**
   * array�^���p�[�X����
   * @remarks �{�֐��I����, �J�[�\����array�^�̎���1�������w��
   */
  Json ParseArray();

  /**
   * �l���`�F�b�N����(�l���s���ȏꍇ�͗�O�𓊂���)
   * @param[in] checkString �`�F�b�N����l
   * @remarks �{�֐��I����, �J�[�\���͒l�̎���1�������w��
   */
  void CheckValue(const std::string& checkString);

  /**
   * value���p�[�X����
   */
  Json ParseValue();
};

//> UNICODE�G�X�P�[�v�V�[�P���X��4BYTE�̈�
const size_t JsonParser::Impl::ESCAPE_SEQUENCE_4BYTE = 4;
//> 16�i���̈�
const int JsonParser::Impl::HEXADECIMAL_16 = 16;
//> UTF8��BOM�T�C�Y
const int JsonParser::Impl::BOM_UTF8_SIZE = 3;
//> UTF8��BOM
const char JsonParser::Impl::BOM_UTF8[] = { char(0xEF), char(0xBB), char(0xBF) };
//> �G���[���b�Z�[�W���i�[����o�b�t�@�T�C�Y
const size_t JsonParser::Impl::BUFFER_SIZE = 256;
//> �G���[���b�Z�[�W --- JSON�̃p�[�X���s
const std::string JsonParser::Impl::ERR_MSG_PARSE_FAILURE("Parse Failure.");
//> �G���[���b�Z�[�W --- 0�ړ���number�^������
const std::string JsonParser::Impl::ERR_MSG_0HEAD_NUMBER("There is '0' Prefixial Number.");
//> �G���[���b�Z�[�W --- number�^�ɖ����ȕ������܂܂�Ă���
const std::string JsonParser::Impl::ERR_MSG_BAD_WORD_IN_NUMBER("There is Bad Word in Number.");
//> �G���[���b�Z�[�W --- number�^(int�^)���͈͊O�̒l
const std::string JsonParser::Impl::ERR_MSG_INT_NUMBER_OUT_OF_RANGE("\"%s\" is out of Integer Range.");
//> �G���[���b�Z�[�W --- number�^(double�^)���͈͊O�̒l
const std::string JsonParser::Impl::ERR_MSG_DOUBLE_NUMBER_OUT_OF_RANGE("\"%s\" is out of Double Range.");
//> �G���[���b�Z�[�W --- '.'�̌㑱�ɐ��l������
const std::string JsonParser::Impl::ERR_MSG_NOT_DIGIT_FOLLOWER_DOT("There is not '0'~'9' Follower of '.' in Number.");
//> �G���[���b�Z�[�W --- 'e', 'E'�̌㑱�ɐ��l������
const std::string JsonParser::Impl::ERR_MSG_NOT_DIGIT_FOLLOWER_EXP("There is not '0'~'9' Follower of 'e' or 'E' in Number.");
//> �G���[���b�Z�[�W --- ����������'"'���Ȃ�
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION("There is not '\"' end of \"%s\".");
//> �G���[���b�Z�[�W --- �s���ȃG�X�P�[�v����������
const std::string JsonParser::Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE("Escape Character of \"%s\" is Failure.");
//> �G���[���b�Z�[�W --- �T���Q�[�g�̈��UNICODE������
const std::string JsonParser::Impl::ERR_MSG_EXIST_CODE_IN_SURROGATE("There is code in the range U+D800 ~ U+DFFF (surrogate).");
//> �G���[���b�Z�[�W --- object�����'}'���Ȃ�
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_OBJ("There is not '}' in object.");
//> �G���[���b�Z�[�W --- object����2�ڈȍ~�̒l�̑O��','���Ȃ�
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_COMMA_IN_OBJ("There is not ',' in object.");
//> �G���[���b�Z�[�W --- object�ɕ�����J�n��'"'���Ȃ�
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION_IN_OBJ("There is not '\"' in object.");
//> �G���[���b�Z�[�W --- object��':'���Ȃ�
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_COLON_IN_OBJ("There is not ':' in object.");
//> �G���[���b�Z�[�W --- array�����']'���Ȃ�
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_ARRAY("There is not ']' in array.");
//> �G���[���b�Z�[�W --- array����2�ڈȍ~�̒l�̑O��','���Ȃ�
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_COMMA_IN_ARRAY("There is not ',' in array.");
//> �G���[���b�Z�[�W --- �s���Ȓl������
const std::string JsonParser::Impl::ERR_MSG_VALUE_FAILURE("Value \"%s\" is Failure.");
//> ���[�j���O���b�Z�[�W --- object��name���d�����Ă���
const std::string JsonParser::Impl::WRN_MSG_NAME_REPEATED("Name \"%s\" Repeated, and Later Name is Given Priority.");

/**
 * �G���[���b�Z�[�W���쐬����
 * @param[in] messageTemp �G���[���b�Z�[�W
 * @param[in] keyWord     ���b�Z�[�W�ɖ��ߍ��ރL�[���[�h
 * @return �G���[���b�Z�[�W
 * @remarks messageTemp�́u%s�v�̉ӏ���keyWord�𖄂ߍ���ŕԂ�
 */
const std::string JsonParser::Impl::MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const
{
  // �o�b�t�@��p��
  char buf[Impl::BUFFER_SIZE];
  // �G���[���b�Z�[�W���쐬���ĕԂ�
  sprintf_s(buf, sizeof(buf), messageTemp.c_str(), keyWord.c_str());
  return std::string(buf);
}

/**
 * ����1�������擾(�z���C�g�X�y�[�X�̓X�L�b�v)
 * @param[out] word �i�[��
 * @return ���^�[���R�[�h(�擾��������SUCCESS)
 * @remarks �{�֐��I����, �J�[�\���͎���1�������w��(�z���C�g�X�y�[�X�̓X�L�b�v)
 */
JsonParser::Impl::RETURN_CODE JsonParser::Impl::SkipWhitespaceAndGetWord(char* word)
{
  // ������������Ύ擾���s
  if (m_i >= m_target.size())
  {
    return FAILURE;
  }
  while (m_target[m_i] == ' ' // ��
    || m_target[m_i] == '\r'  // ���A
    || m_target[m_i] == '\n'  // ���s
    || m_target[m_i] == '\t') // �^�u
  {
    ++m_i;
    // ������������Ύ擾���s
    if (m_i >= m_target.size())
    {
      return FAILURE;
    }
  }
  // �������i�[
  *word = m_target[m_i];
  ++m_i;
  // �擾����
  return SUCCESS;
}

/**
 * UTF8��BOM������΃X�L�b�v����
 */
void JsonParser::Impl::SkipBOM()
{
  // BOM���擾
  std::string bom(m_target.substr(m_i, BOM_UTF8_SIZE));
  // UTF8��BOM�T�C�Y���擾�ł��Ȃ������ꍇ��BOM�Ȃ��Ɣ��f���Ė߂�
  if (bom.size() != BOM_UTF8_SIZE) return;
  // BOM�`�F�b�N
  for (int i = 0; i < BOM_UTF8_SIZE; ++i)
  {
    // int�^�ɕϊ����ă`�F�b�N(UTF8��BOM�łȂ���Ζ߂�)
    if (BOM_UTF8[i] != bom[i]) return;
  }
  // �����܂ŗ�����bom�Ȃ̂ŃX�L�b�v
  m_i += BOM_UTF8_SIZE;
}

/**
 * �G�X�P�[�v�V�[�P���X�𕶎���ɕϊ�����
 * @param[in] hex UNICODE16�i��
 */
const std::string JsonParser::Impl::ConversionEscapeSequence(int hex)
{
  // �Ԃ�������
  std::string returnString("");

  // ASCII 0x0000 ~ 0x007F (0000|0000|0xxx|xxxx)
  if (hex <= 0x007F)
  {
    // ��1�o�C�g 0xxx|xxxx
    returnString += static_cast<char>(hex);
  }
  // 0x0080 ~ 0x07FF (0000|0xxx|xxyy|yyyy)
  else if (hex <= 0x07FF)
  {
    // ��1�o�C�g 110x|xxxx
    // (hex >> 6)�� 0000|0000|000x|xxxx
    // (| 0x00C0)�� 0000|0000|1100|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>((hex >> 6) | 0x00C0);
    // ��2�o�C�g 10yy|yyyy
    // (& 0x003F)�� 0000|0000|0011|1111 ��0�̉ӏ������O
    // (| 0x0080)�� 0000|0000|1000|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>((hex & 0x003F) | 0x0080);
  }
  // 0x0800 ~ 0xD7FF (xxxx|yyyy|yyzz|zzzz)
  else if (hex <= 0xD7FF)
  {
    // ��1�o�C�g 1110|xxxx
    // (hex >>12)�� 0000|0000|0000|xxxx
    // (| 0x00E0)�� 0000|0000|1110|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>((hex >> 12) | 0x00E0);
    // ��2�o�C�g 10yy|yyyy
    // (hex >> 6)�� 0000|00xx|xxyy|yyyy
    // (& 0x003F)�� 0000|0000|0011|1111 ��0�̉ӏ������O
    // (| 0x0080)�� 0000|0000|1000|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>(((hex >> 6) & 0x003F) | 0x0080);
    // ��3�o�C�g 10zz|zzzz
    // (& 0x003F)�� 0000|0000|0011|1111 ��0�̉ӏ������O
    // (| 0x0080)�� 0000|0000|1000|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>((hex & 0x003F) | 0x0080);
  }
  // �T���Q�[�g�̈� 0x0800 ~ 0xDFFF (�g�p�֎~)
  else if (hex <= 0xDFFF)
  {
    // �G���[���b�Z�[�W�𓊂���
    throw Impl::ERR_MSG_EXIST_CODE_IN_SURROGATE;
  }
  // 0xE000 ~ 0xFFFF (xxxx|yyyy|yyzz|zzzz)
  else if (hex <= 0xFFFF)
  {
    // ��1�o�C�g 1110|xxxx
    // (hex >>12)�� 0000|0000|0000|xxxx
    // (| 0x00E0)�� 0000|0000|1110|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>((hex >> 12) | 0x00E0);
    // ��2�o�C�g 10yy|yyyy
    // (hex >> 6)�� 0000|00xx|xxyy|yyyy
    // (& 0x003F)�� 0000|0000|0011|1111 ��0�̉ӏ������O
    // (| 0x0080)�� 0000|0000|1000|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>(((hex >> 6) & 0x003F) | 0x0080);
    // ��3�o�C�g 10zz|zzzz
    // (& 0x003F)�� 0000|0000|0011|1111 ��0�̉ӏ������O
    // (| 0x0080)�� 0000|0000|1000|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>((hex & 0x003F) | 0x0080);
  }
  // 0x010000 ~ 0x10FFFF (x|xxyy|yyyy|zzzz|zzww|wwww)
  else
  {
    // ��1�o�C�g 1111|0xxx
    // (hex >>18)�� 0000|0000|0000|0xxx
    // (| 0x00F0)�� 0000|0000|1111|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>((hex >> 18) | 0x00F0);
    // ��2�o�C�g 10yy|yyyy
    // (hex >>12)�� 0000|000x|xxyy|yyyy
    // (& 0x003F)�� 0000|0000|0011|1111 ��0�̉ӏ������O
    // (| 0x0080)�� 0000|0000|1000|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>(((hex >> 12) & 0x003F) | 0x0080);
    // ��3�o�C�g 10zz|zzzz
    // (hex >> 6)�� 0xxx|yyyy|yyzz|zzzz
    // (& 0x003F)�� 0000|0000|0011|1111 ��0�̉ӏ������O
    // (| 0x0080)�� 0000|0000|1000|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>(((hex >> 6) & 0x003F) | 0x0080);
    // ��4�o�C�g 10ww|wwww
    // (& 0x003F)�� 0000|0000|0011|1111 ��0�̉ӏ������O
    // (| 0x0080)�� 0000|0000|1000|0000 ��1�̉ӏ���t��
    returnString += static_cast<char>((hex & 0x003F) | 0x0080);
  }

  return returnString;
}

/**
 * string�^���p�[�X����
 * @remarks �{�֐��I����, �J�[�\����string�^�̎���1�������w��
 */
Json JsonParser::Impl::ParseString()
{
  // <string>
  //          
  // *--->'"'------------------------------------->'"'--->*
  //          |                                 ^
  //          |                                 |
  //          |<--------------------------------|
  //          |                                 |
  //          |                                 | 
  //          |------> UNICODE character ------>|
  //          |        (except '"' or '\')      |
  //          |                                 |
  //          V                                 | 
  //          --->'\'----->'"'----------------->| quotation mark
  //                   |                        |
  //                   |-->'\'----------------->| reverse solidus
  //                   |-->'/'----------------->| solidus
  //                   |-->'b'----------------->| backspace
  //                   |-->'f'----------------->| formfeed
  //                   |-->'n'----------------->| newline
  //                   |-->'r'----------------->| carriage return
  //                   |-->'t'----------------->| horizontal tab
  //                   V-->'u'-->4 hex digits-->| escape sequence
  //

  // �o�b�t�@��p��
  std::string str("");

  // �ŏ���'"'���X�L�b�v
  ++m_i;

  // '"'������܂Ń��[�v
  while (true)
  {
    // ������̍Ō�𒴂�����G���[
    if (m_i >= m_target.size())
    {
      // �G���[���b�Z�[�W�𓊂���
      throw MakeErrorMsg(Impl::ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION, '\"' + str);
    }
    // 1�����擾
    char word = m_target[m_i];
    ++m_i;

    // '"'��������I��
    if (word == '\"')
    {
      // ���[�v�𔲂���
      break;
    }

    //----- �G�X�P�[�v�����łȂ��ꍇ -----//
    if (word != '\\')
    {
      // �i�[���Ď���
      str += word;
      continue;
    }

    //----- �G�X�P�[�v�����̏ꍇ -----//
    // ������̍Ō�𒴂��Ă�����G���[
    if (m_i >= m_target.size())
    {
      // �G���[���b�Z�[�W�𓊂���
      str += word;
      throw MakeErrorMsg(Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE, '\"' + str);
    }

    // 1�����擾
    word = m_target[m_i];
    ++m_i;

    // �G�X�P�[�v��������
    switch (word)
    {
      case '\"': // quotation mark
      case '\\': // reverse solidus
      case '/':  // solidus
        str += word;
        break;
      case 'b': // backspace
        str += '\b';
        break;
      case 'f': // formfeed
        str += '\f';
        break;
      case 'n': // newline
        str += '\n';
        break;
      case 'r': // carriage return
        str += '\r';
        break;
      case 't': // horizontal tab
        str += '\t';
        break;
      case 'u': // escape sequence
      {
        // �G�X�P�[�v�V�[�P���X�̒l4BYTE���擾
        std::string escapeSequence = m_target.substr(m_i, ESCAPE_SEQUENCE_4BYTE);
        m_i += escapeSequence.size();
        // 4BYTE�擾�ł��Ȃ���΃G���[
        if (escapeSequence.size() < ESCAPE_SEQUENCE_4BYTE)
        {
          // �G���[���b�Z�[�W�𓊂���
          str += word;
          throw MakeErrorMsg(Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE, '\"' + str);
        }
        // 16�i���łȂ���΃G���[
        for (int i = 0; i < ESCAPE_SEQUENCE_4BYTE; ++i)
        {
          if (escapeSequence[i] >= '0' && escapeSequence[i] <= '9') continue;
          if (escapeSequence[i] >= 'a' && escapeSequence[i] <= 'f') continue;
          if (escapeSequence[i] >= 'A' && escapeSequence[i] <= 'F') continue;
          // �G���[���b�Z�[�W�𓊂���
          str += word;
          throw MakeErrorMsg(Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE, '\"' + str);
        }
        // int�^(10�i��)�ɕϊ�����
        int hex = std::stoi(escapeSequence, nullptr, Impl::HEXADECIMAL_16);
        // �G�X�P�[�v�V�[�P���X��ϊ�����str�ɉ��Z����
        str += ConversionEscapeSequence(hex);
      }
      break;
      default:
        // �G���[���b�Z�[�W�𓊂���
        str += word;
        throw MakeErrorMsg(Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE, '\"' + str);
    }
  }

  //***** string�^Json�I�u�W�F�N�g��Ԃ� *****//
  return Json(str);
}

/**
 * number�^���p�[�X����
 * @remarks �{�֐��I����, �J�[�\����number�^�̎���1�������w��
 */
Json JsonParser::Impl::ParseNumber()
{
  // <number>                               <------------
  //                                        |           ^
  //                               (double) V           |                       
  //                               ---'.'----->'0'~'9'-->
  //                               ^                    |
  //                               |                    V
  // *----------------->'0'---------------------------------------------------------------------->*
  //    |       ^  |           ^                          |                                     ^
  //    V       |  V           |                          V (double)                            |
  //    -->'-'-->  -->'1'~'9'-->                          --> 'e', 'E' --> '+', '-'---->'0'~'9'->
  //               ^           |                                        |          ^ ^          |
  //               |           V                                        V          | |          V
  //               <--'0'~'9'<--                                        -----------> <-----------
  //

  // �J�n�C���f�b�N�X������
  size_t start = m_i;

  // '-'�m�F
  if (m_target[m_i] == '-') ++m_i;

  // '0'�m�F
  if (m_target[m_i] == '0')
  {
    ++m_i;
    // '0'����n�܂�number�^�Ŏ���'0'~'9'�ł���Ζ���
    if ('0' <= m_target[m_i] && m_target[m_i] <= '9')
    {
      // �G���[���b�Z�[�W�𓊂���
      throw Impl::ERR_MSG_0HEAD_NUMBER;
    }
  }
  // '1'~'9'�m�F
  else if ('1' <= m_target[m_i] && m_target[m_i] <= '9')
  {
    ++m_i;
    // �㑱��'0'~'9'���m�F
    while ('0' <= m_target[m_i] && m_target[m_i] <= '9') ++m_i;
  }
  // ���̑��̓G���[
  else
  {
    // �G���[���b�Z�[�W�𓊂���
    throw Impl::ERR_MSG_BAD_WORD_IN_NUMBER;
  }

  // int�^���m�F
  if (m_target[m_i] != '.'
    && m_target[m_i] != 'e'
    && m_target[m_i] != 'E')
  {
    try
    {
      // int�^�ɕϊ�(C+11STL�ł����, string�̓����o�b�t�@�͘A���̈�ɔz�u�����)
      int number = std::stoi(m_target.c_str() + start);
      //***** int�^Json�I�u�W�F�N�g��Ԃ� *****//
      return Json(number);
    }
    // �͈͊O�̒l�ɂȂ����ꍇ
    catch (std::out_of_range e)
    {
      // �G���[���b�Z�[�W�𓊂���
      throw MakeErrorMsg(Impl::ERR_MSG_INT_NUMBER_OUT_OF_RANGE, m_target.c_str() + start);
    }
    // ���l�ւ̕ϊ����s���Ȃ��ꍇ
    catch (std::invalid_argument e)
    {
      // �G���[���b�Z�[�W�𓊂���
      throw Impl::ERR_MSG_BAD_WORD_IN_NUMBER;
    }
  }

  // '.'�m�F
  if (m_target[m_i] == '.')
  {
    ++m_i;
    // �㑱��'0'~'9'��������΃G���[
    if (m_target[m_i] < '0' || '9' < m_target[m_i])
    {
      // �G���[���b�Z�[�W�𓊂���
      throw Impl::ERR_MSG_NOT_DIGIT_FOLLOWER_DOT;
    }
    // �㑱��'0'~'9'���m�F
    while ('0' <= m_target[m_i] && m_target[m_i] <= '9') ++m_i;
  }

  // 'e', 'E'�m�F
  if (m_target[m_i] == 'e' || m_target[m_i] == 'E')
  {
    ++m_i;
    // '+', '-'�m�F
    if (m_target[m_i] == '+' || m_target[m_i] == '-') ++m_i;
    // �㑱��'0'~'9'��������΃G���[
    if (m_target[m_i] < '0' || '9' < m_target[m_i])
    {
      // �G���[���b�Z�[�W�𓊂���
      throw Impl::ERR_MSG_NOT_DIGIT_FOLLOWER_EXP;
    }
    // �㑱��'0'~'9'���m�F
    while ('0' <= m_target[m_i] && m_target[m_i] <= '9') ++m_i;
  }

  try
  {
    // double�^�ɕϊ�(C+11STL�ł����, string�̓����o�b�t�@�͘A���̈�ɔz�u�����)
    double number = std::stod(m_target.c_str() + start);
    //***** double�^Json�I�u�W�F�N�g��Ԃ� *****//
    return Json(number);
  }
  // �͈͊O�̒l�ɂȂ����ꍇ
  catch (std::out_of_range e)
  {
    // �G���[���b�Z�[�W�𓊂���
    throw MakeErrorMsg(Impl::ERR_MSG_DOUBLE_NUMBER_OUT_OF_RANGE, m_target.c_str() + start);
  }
  // ���l�ւ̕ϊ����s���Ȃ��ꍇ
  catch (std::invalid_argument e)
  {
    // �G���[���b�Z�[�W�𓊂���
    throw Impl::ERR_MSG_BAD_WORD_IN_NUMBER;
  }
}

/**
 * object�^���p�[�X����
 * @remarks �{�֐��I����, �J�[�\����object�^�̎���1�������w��
 */
Json JsonParser::Impl::ParseObject()
{
  // <object>
  //          
  // *--->'{'------------------------------------>'}'--->*
  //          |                                ^
  //          |                                |
  //          |<--------------','<-------------|
  //          |                                |
  //          V                                | 
  //          ---> string --->':'---> value --->
  //

  // �o�b�t�@��p��
  Json::Object obj;
  // 1st value�t���O��p��(�ŏ���value�ł����true�ƂȂ�)
  bool flag1stValue = true;

  // '{'���X�L�b�v
  ++m_i;

  // '}'������܂Ń��[�v
  while (true)
  {
    // �����̊i�[���p��
    char word = '\0';
    // �m�F�̂���, ���̕������擾
    if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
    {
      // �G���[���b�Z�[�W�𓊂���
      throw Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_OBJ;
    }

    // '}'��������I��
    if (word == '}')
    {
      // ���[�v�𔲂���
      break;
    }

    // �ŏ���value�łȂ��Ƃ�
    if (flag1stValue == false)
    {
      // ','�łȂ���΃G���[
      if (word != ',')
      {
        // �G���[���b�Z�[�W�𓊂���
        throw Impl::ERR_MSG_NOT_EXIST_COMMA_IN_OBJ;
      }
      // �m�F�̂���, ���̕������擾
      if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
      {
        // �G���[���b�Z�[�W�𓊂���
        throw Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_OBJ;
      }
    }

    // 1�����߂�
    --m_i;
    // string���i�[����o�b�t�@��p��
    std::string key;
    // string�^�ł��邱�Ƃ��m�F���ăp�[�X(Parse�֐��ɑΉ�����悤��switch���ŋL��)
    switch (word)
    {
      case '"': // string�^
      {
        auto stringJson = this->ParseString();
        // ���s����null�^Json�I�u�W�F�N�g��Ԃ�
        if (stringJson.IsNull()) return Json(nullptr);
        // string���擾
        key = stringJson.GetString();
        // �L�[���d�����Ă���ꍇ
        if (obj.count(key) > 0)
        {
          // �d���t���O�����Ă�
          m_nameRepeated = true;
          // ���[�j���O���b�Z�[�W�i�[
          m_warningMsg.emplace_back(
            MakeErrorMsg(Impl::WRN_MSG_NAME_REPEATED, key));
        }
        break;
      }
      default: // others
        // �G���[���b�Z�[�W�𓊂���
        throw Impl::ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION_IN_OBJ;
    }

    // �m�F�̂���, ���̕������擾(':'���擾�ł��Ȃ���΃G���[)
    if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS
      || word != ':')
    {
      // �G���[���b�Z�[�W�𓊂���
      throw Impl::ERR_MSG_NOT_EXIST_COLON_IN_OBJ;
    }

    // �p�[�X���Ċi�[
    obj[key] = this->ParseValue();
    // �ŏ���value��ǂݏI������̂Ńt���O�����낷
    flag1stValue = false;
  }
  //***** object�^Json�I�u�W�F�N�g��Ԃ� *****//
  return Json(obj);
}

/**
 * array�^���p�[�X����
 * @remarks �{�֐��I����, �J�[�\����array�^�̎���1�������w��
 */
Json JsonParser::Impl::ParseArray()
{
  // <array>
  //          
  // *--->'['------------------------------------>']'--->*
  //          |                                ^
  //          |                                |
  //          |<--------------','<-------------|
  //          |                                |
  //          V                                | 
  //          -------------> value ------------>
  //

  // �o�b�t�@��p��
  Json::Array arr;
  // 1st value�t���O��p��(�ŏ���value�ł����true�ƂȂ�)
  bool flag1stValue = true;

  // '['���X�L�b�v
  ++m_i;

  // ']'������܂Ń��[�v
  while (true)
  {
    // �����̊i�[���p��
    char word = '\0';
    // �m�F�̂���, ���̕������擾
    if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
    {
      // �G���[���b�Z�[�W�𓊂���
      throw Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_ARRAY;
    }

    // ']'��������I��
    if (word == ']')
    {
      // ���[�v�𔲂���
      break;
    }

    // �ŏ���value�łȂ��Ƃ�
    if (flag1stValue == false)
    {
      // ','�łȂ���΃G���[
      if (word != ',')
      {
        // �G���[���b�Z�[�W�𓊂���
        throw Impl::ERR_MSG_NOT_EXIST_COMMA_IN_ARRAY;
      }
      // �m�F�̂���, ���̕������擾
      if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
      {
        // �G���[���b�Z�[�W�𓊂���
        throw Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_ARRAY;
      }
    }

    // 1�����߂�
    --m_i;
    // �p�[�X���Ċi�[
    arr.emplace_back(this->ParseValue());
    // �ŏ���value��ǂݏI������̂Ńt���O�����낷
    flag1stValue = false;
  }
  //***** array�^Json�I�u�W�F�N�g��Ԃ� *****//
  return Json(arr);
}

/**
 * �l���`�F�b�N����(�l���s���ȏꍇ�͗�O�𓊂���)
 * @param[in] checkString �`�F�b�N����l
 * @remarks �{�֐��I����, �J�[�\���͒l�̎���1�������w��
 */
void JsonParser::Impl::CheckValue(const std::string& checkString)
{
  for (size_t i = 0; i < checkString.size(); ++i)
  {
    // ������̍Ō�𒴂��Ă���Ƃ��̓G���[
    if (m_i >= m_target.size())
    {
      // �G���[���b�Z�[�W�𓊂���
      throw MakeErrorMsg(JsonParser::Impl::ERR_MSG_VALUE_FAILURE, m_target.substr(m_i - 1 - i, i - 1));
    }
    // �����񂪒l�ƈ�v���Ȃ��Ƃ��̓G���[
    if (m_target[m_i] != checkString[i])
    {
      // �G���[���b�Z�[�W�𓊂���
      throw MakeErrorMsg(JsonParser::Impl::ERR_MSG_VALUE_FAILURE, m_target.substr(m_i - i, i));
    }
    // �G���[�łȂ���Ύ����m�F����
    ++m_i;
  }
}

/**
 * value���p�[�X����
 */
Json JsonParser::Impl::ParseValue()
{
  // �����̊i�[���p��
  char word = '\0';

  // �m�F�̂���, ���̕������擾
  if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
  {
    // �擾�ł��Ȃ������ꍇ�̓G���[���b�Z�[�W�𓊂���
    throw Impl::ERR_MSG_PARSE_FAILURE;
  }
  // 1�����߂�
  --(this->m_i);

  // value�͎��̂悤�ɕ��򂷂�
  //
  // *----> string�^ ----->*
  //    |              ^
  //    V              |
  //    --> number�^ -->
  //    |              ^
  //    V              |
  //    --> object�^ -->
  //    |              ^
  //    V              |
  //    --> array�^  -->
  //    |              ^
  //    V              |
  //    --> true     -->
  //    |              ^
  //    V              |
  //    --> false    -->
  //    |              ^
  //    V              |
  //    --> null �@  -->
  //
  switch (word)
  {
    case '"': // string�^
      return this->ParseString();

    case '-': // number�^
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return this->ParseNumber();

    case '{': // object�^
      return this->ParseObject();

    case '[': // array�^
      return this->ParseArray();

    case 't': // true
      // true�ł��邱�Ƃ��`�F�b�N���Ă���, bool�^Json�I�u�W�F�N�g(true)��Ԃ�
      this->CheckValue("true");
      return Json(true);

    case 'f': // false
      // false�ł��邱�Ƃ��`�F�b�N���Ă���, bool�^Json�I�u�W�F�N�g(false)��Ԃ�
      this->CheckValue("false");
      return Json(false);

    case 'n': // null
      // null�ł��邱�Ƃ��`�F�b�N���Ă���, null��Json�I�u�W�F�N�g��Ԃ�
      this->CheckValue("null");
      return Json(nullptr);

    default: // others
      // �G���[���b�Z�[�W�𓊂���
      throw JsonParser::Impl::ERR_MSG_PARSE_FAILURE;
  }
}

/** �R���X�g���N�^ */
JsonParser::JsonParser()
  : m_impl(std::make_unique<Impl>(""))
{
}

/** �f�X�g���N�^ */
JsonParser::~JsonParser() {}

/**
 * JSON�`���̕�������p�[�X����
 * @param[in] target �p�[�X�Ώۂ̕�����
 * @return Json�I�u�W�F�N�g
 * @attention ����܂ł̃G���[���b�Z�[�W�⃏�[�j���O���b�Z�[�W�������������
 */
Json JsonParser::Parse(const std::string& target)
{
  // Pimpl��������
  m_impl = std::make_unique<Impl>(target);
  // UTF8��BOM������΃X�L�b�v����
  m_impl->SkipBOM();

  try
  {
    // �p�[�X����
    return m_impl->ParseValue();
  }
  catch (const std::string& err)
  {
    // �G���[���b�Z�[�W�i�[
    m_impl->m_errorMsg.emplace_back(err);
    // ���s����null�^Json�I�u�W�F�N�g��Ԃ�
    return Json(nullptr);
  }
}

/**
 * �G���[���b�Z�[�W���擾����
 * @return �G���[���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
 */
const std::vector<std::string>& JsonParser::ErrorMessage() const
{
  return m_impl->m_errorMsg;
}

/**
 * ���[�j���O���b�Z�[�W���擾����
 * @return ���[�j���O���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
 */
const std::vector<std::string>& JsonParser::WarningMessage() const
{
  return m_impl->m_warningMsg;
}

/**
 * object��name���d�����Ă����true��Ԃ�
 * @return �O���Parse��object��name���d�����Ă����true��Ԃ�
 */
bool JsonParser::IsNameRepeated() const
{
  return m_impl->m_nameRepeated;
}
}
