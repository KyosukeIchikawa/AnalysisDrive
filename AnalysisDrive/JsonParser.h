/**
 * @file    JsonParser.h
 * @brief   JSON�p�[�T�[
 * @author  kyo
 * @date    2015/05/07
 */
#ifndef ANALYSISDRIVE_JSONPARSER_H_
#define ANALYSISDRIVE_JSONPARSER_H_

#include <memory>
#include <string>
#include <vector>

/** Analysis Drive */
namespace adlib
{
// �O���錾
class Json;

/**
 * JSON�p�[�T�[
 */
class JsonParser
{
private:
  //> Pimpl�C�f�B�I��
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /* �R�s�[�֎~ */
  JsonParser(const JsonParser&);
  /* ����֎~ */
  void operator=(const JsonParser&);

public:
  /** �R���X�g���N�^ */
  JsonParser();
  /** ���z�f�X�g���N�^ */
  virtual ~JsonParser();

  /**
   * JSON�`���̕�������p�[�X����
   * @param[in] target �p�[�X�Ώۂ̕�����
   * @return Json�I�u�W�F�N�g
   * @attention ����܂ł̃G���[���b�Z�[�W�⃏�[�j���O���b�Z�[�W�������������
   */
  Json Parse(const std::string& target);

  /**
   * �G���[���b�Z�[�W���擾����
   * @return �G���[���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
   */
  const std::vector<std::string>& ErrorMessage() const;

  /**
   * ���[�j���O���b�Z�[�W���擾����
   * @return ���[�j���O���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
   */
  const std::vector<std::string>& WarningMessage() const;

  /**
   * object��name���d�����Ă����true��Ԃ�
   * @return �O���Parse��object��name���d�����Ă����true��Ԃ�
   */
  bool IsNameRepeated() const;
};

/**
 * @class   JsonParser
 * @section JsonParser_exsample exsample
 * @code
 * #include "AnalysisDrive.h"
 * #include <fstream>
 * #include <iostream>
 *
 * // ���̃f�[�^���t�@�C����"test.txt"�ŗp��
 * // �� �����R�[�h��UTF8(BOM�L���͖��Ȃ�)
 * //
 * // {
 * //   "comment": "test data",
 * //   "method": {"analysis": "FEM", "timestep": 100000, "stepsize": 0.0001},
 * //   "nonlinear": false,
 * //   "node":
 * //   [
 * //     {"coord": [0.0, 0.0, 0.0], "comment": "Origin"},
 * //     {"coord": [2.0, 0.0, 0.0]},
 * //     {"coord": [0.0, 1.2, 0.0]},
 * //     {"coord": [2.0, 1.2, 0.0]}
 * //   ]
 * // }
 * //
 * const std::string file("test.txt");
 *
 * int main()
 * {
 *   // �t�@�C����ǂݍ���
 *   std::ifstream ifs;
 *   ifs.open(file);
 *
 *   // �o�b�t�@��JSON�̃f�[�^���i�[
 *   std::string buf("");
 *   std::string tmp("");
 *   while (std::getline(ifs, tmp)) buf += tmp;
 *
 *   // �p�[�T�[��p��
 *   adlib::JsonParser parser;
 *
 *   // �p�[�X����JSON�I�u�W�F�N�g���擾
 *   auto json = parser.Parse(buf);
 *   // �G���[�m�F
 *   if (parser.ErrorMessage().size() > 0)
 *   {
 *     std::cout << "ERROR" << std::endl;
 *     return -1;
 *   }
 *
 *   // json��object�^�łȂ���΃G���[
 *   if (!(json.IsObject())) return -1;
 *   // object���擾
 *   auto jsonObj = json.GetObject();
 *
 *   // "method"������, object�ł��邱�Ƃ��m�F
 *   if (jsonObj["method"].IsObject())
 *   {
 *     // method�̒l��object�Ƃ��Ď擾
 *     auto method = jsonObj["method"].GetObject();
 *
 *     // "analysis"������, string�ł���Ώo��
 *     if (method["analysis"].IsString()) std::cout << "analysis " << method["analysis"].GetString() << std::endl;
 *
 *     // "timestep"������, int��������double�ł����int�^�Ƃ��ďo��
 *     if (method["timestep"].IsNumber()) std::cout << "timestep " << method["timestep"].GetNumberInt() << std::endl;
 *
 *     // "stepsize"������, int��������double�ł����double�^�Ƃ��ďo��
 *     if (method["stepsize"].IsNumber()) std::cout << "stepsize " << method["stepsize"].GetNumberDouble() << std::endl; *
 *
 *   }
 *
 *   // "node"������, array�ł��邱�Ƃ��m�F
 *   if (jsonObj["node"].IsArray())
 *   {
 *     // node�̒l��array�Ƃ��Ď擾
 *     auto nodes = jsonObj["node"].GetArray();
 *     // array�̐������������[�v����
 *     // �� nodes.size()�ŃT�C�Y���擾����, �C���f�b�N�X�Ń��[�v���܂킵�Ă��ǂ�
 *     for (auto node = nodes.begin(); node != nodes.end(); ++node)
 *     {
 *       // object�ł��邱�Ƃ��m�F
 *       if (node->IsObject())
 *       {
 *         auto nodeObj = node->GetObject();
 *         // "coord"������, array�ł��邱�Ƃ��m�F
 *         if (nodeObj["coord"].IsArray())
 *         {
 *           // coord�̒l��array�Ƃ��Ď擾
 *           auto coords = nodeObj["coord"].GetArray();
 *           // �S�v�f�o��
 *           std::cout << "coord " << std::flush;
 *           for (auto coord = coords.begin(); coord != coords.end(); ++coord)
 *           {
 *             std::cout << coord->GetNumberDouble() << std::flush;
 *           }
 *           std::cout << std::endl;
 *         }
 *       }
 *       else
 *       {
 *         continue;
 *       }
 *     }
 *   }
 *
 *   // "linear"�͂Ȃ����Ƃ��m�F
 *   if (jsonObj["linear"].IsNull())
 *   {
 *     std::cout << "linear ????" << std::endl;
 *   }
 *
 *   return 0;
 * }

 * @endcode
 */
}
#endif // ANALYSISDRIVE_JSONPARSER_H_
