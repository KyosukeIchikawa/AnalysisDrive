/**
 * @file   Json.h
 * @brief  JSON�I�u�W�F�N�g
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_JSON_H_
#define ANALYSISDRIVE_JSON_H_

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <ostream>

/** Analysis Drive */
namespace adlib
{
/** JSON�I�u�W�F�N�g */
class Json final
{
public:
  //! JSON�̌^
  enum JSON_VALUE_TYPE
  {
    NUL,           /*!< null           */
    STRING,        /*!< string         */
    NUMBER_INT,    /*!< number(int)    */
    NUMBER_DOUBLE, /*!< number(double) */
    BOOL,          /*!< bool           */
    ARRAY,         /*!< array          */
    OBJECT         /*!< object         */
  };

private:
  //! Pimpl�C�f�B�I��
  class Impl;
  std::unique_ptr<Impl> m_impl;

public:
  /**
   * �R���X�g���N�^
   * @remarks �����Ȃ��̃R���X�g���N�^�ł�null��JSON�I�u�W�F�N�g���쐬����
   * @note std::map<std::string, Json>�𑶍݂��Ȃ��L�[�ŃA�N�Z�X�����ꍇ, <br>
   *       �{�R���X�g���N�^�ɂ����Json���V�K�o�^�����. <br>
   *       ����ɂ��, ���݂��Ȃ��L�[�ŃA�N�Z�X�����null�Ɠ��l�̒l�ɂȂ�.
   */
  Json();

  /**
   * �R���X�g���N�^
   * @param[in] value �l(null)
   */
  explicit Json(std::nullptr_t value);

  /**
   * �R���X�g���N�^
   * @param[in] value �l(string�^)
   */
  explicit Json(const std::string& value);

  /**
   * �R���X�g���N�^
   * @param[in] value �l(number�^(int))
   */
  explicit Json(int value);

  /**
   * �R���X�g���N�^
   * @param[in] value �l(number�^(double))
   */
  explicit Json(double value);

  /**
   * �R���X�g���N�^
   * @param[in] value �l(bool�^)
   */
  explicit Json(bool value);

  /**
   * �R���X�g���N�^
   * @param[in] value �l(array�^)
   */
  explicit Json(const std::vector<Json>& value);

  /**
   * �R���X�g���N�^
   * @param[in] value �l(object�^)
   */
  explicit Json(const std::map<std::string, Json>& value);

  /**
   * �R�s�[�R���X�g���N�^
   * @param[in] obj �R�s�[��
   */
  Json(const Json& obj);

  /**
   * ���[�u�R���X�g���N�^
   * @param[in] obj ���[�u�I�u�W�F�N�g
   */
  Json(Json&& obj);

  /**
   * ���
   * @param[in] obj �������I�u�W�F�N�g
   */
  Json& operator=(const Json& obj);

  /** ���z�f�X�g���N�^ */
  virtual ~Json();

  /**
   * �^���擾
   * @return �^
   */
  JSON_VALUE_TYPE GetValueType() const;

  /**
   * null���ǂ���
   * @return null�Ȃ�true
   */
  bool IsNull() const;

  /**
   * string���ǂ���
   * @return string�Ȃ�true
   */
  bool IsString() const;

  /**
   * number(int or double)���ǂ���
   * @return number�Ȃ�true
   */
  bool IsNumber() const;

  /**
   * number(int)���ǂ���
   * @return number(int)�Ȃ�true
   */
  bool IsNumberInt() const;

  /**
   * number(double)���ǂ���
   * @return number(double)�Ȃ�true
   */
  bool IsNumberDouble() const;

  /**
   * bool���ǂ���
   * @return bool�Ȃ�true
   */
  bool IsBool() const;

  /**
   * array���ǂ���
   * @return array�Ȃ�true
   */
  bool IsArray() const;

  /**
   * object���ǂ���
   * @return object�Ȃ�true
   */
  bool IsObject() const;

  /**
   * string�l��get
   * @return string�l
   */
  const std::string& GetString() const;

  /**
   * number�l(int)��get
   * @return number�l(int)
   */
  int GetNumberInt() const;

  /**
   * number(double)��get
   * @return number�l(double)
   */
  double GetNumberDouble() const;

  /**
   * bool�l��get
   * @return bool�l
   */
  bool GetBool() const;

  /**
   * array�l��get
   * @return array�l
   */
  std::vector<Json>& GetArray();

  /**
   * array�l��get(const��)
   * @return array�l
   */
  const std::vector<Json>& GetArray() const;

  /**
   * object�l��get
   * @return object�l
   */
  std::map<std::string, Json>& GetObject();

  /**
   * object�l��get(const��)
   * @return object�l
   */
  const std::map<std::string, Json>& GetObject() const;

  /**
   * JSON�`���̕�������o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void Dump(std::ostream* outStream) const;

  /**
   * MessagePack�`���̃o�C�i�����o��
   * @param[in,out] outStream �o�͐�̃X�g���[��
   */
  void DumpMsgpack(std::ostream* outStream) const;
};
}
#endif // ANALYSISDRIVE_JSON_H_
