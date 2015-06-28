/**
 * @file   JsonValue.h
 * @brief  JSON�̒l
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUE_H_
#define ANALYSISDRIVE_JSONVALUE_H_

#include "Json.h"
#include <string>
#include <vector>
#include <map>

/** Analysis Drive */
namespace adlib
{
/**
 * JSON�̒l
 * @remarks ���ۃN���X
 */
class JsonValue
{
public:
  /** ���z�f�X�g���N�^ */
  virtual ~JsonValue() {}

  /**
   * �^���擾
   * @return �^
   */
  virtual Json::JSON_VALUE_TYPE GetValueType() const = 0;

  /**
   * string�l��get
   * @return string�l
   * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
   *          �󕶎�""��Ԃ�
   */
  virtual const std::string& GetString() const;

  /**
   * number�l(int)��get
   * @return number�l(int)
   * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
   *          0��Ԃ�
   */
  virtual int GetNumberInt() const;

  /**
   * number�l(double)��get
   * @return number�l(double)
   * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
   *          0.0��Ԃ�
   */
  virtual double GetNumberDouble() const;

  /**
   * bool�l��get
   * @return bool�l
   * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
   *          false��Ԃ�
   */
  virtual bool GetBool() const;

  /**
   * array�l��get
   * @return array�l
   * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
   *          ���vector��Ԃ�
   */
  virtual std::vector<Json>& GetArray();

  /**
   * array�l��get(const��)
   * @return array�l
   * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
   *          ���vector��Ԃ�
   */
  virtual const std::vector<Json>& GetArray() const;

  /**
   * object�l��get
   * @return object�l
   * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
   *          ���map��Ԃ�
   */
  virtual std::map<std::string, Json>& GetObject();

  /**
   * object�l��get(const��)
   * @return object�l
   * @remarks �I�[�o�[���C�h����Ȃ��ꍇ�̓��� <br>
   *          ���map��Ԃ�
   */
  virtual const std::map<std::string, Json>& GetObject() const;
};
}
#endif // ANALYSISDRIVE_JSONVALUE_H_
