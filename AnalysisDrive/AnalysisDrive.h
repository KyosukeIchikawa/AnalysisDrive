/**
 * @file   AnalysisDrive.h
 * @brief  Analysis Drive Include Header File
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_ANALYSISDRIVE_H_
#define ANALYSISDRIVE_ANALYSISDRIVE_H_

//------------------------------------------------------------------------------
// Include Files
//------------------------------------------------------------------------------
/* �R�}���h���C������ */
#include "CommandLineSetting.h" // �R�}���h���C���ǂݍ��ݐݒ�
#include "CommandLineParser.h"  // �R�}���h���C���ǂݍ��݋@�\
#include "Command.h"            // �R�}���h���C���ɓ��͂��ꂽ�����̏��
/* JSON���� */
#include "Json.h"               // JSON�I�u�W�F�N�g
#include "JsonParser.h"         // JSON�p�[�T�[
/* MessgaePack���� */
#include "MsgpackParser.h"      // MessagePack�p�[�T�[
/* ���̑� */
#include "EasyTimer.h"          // ���Ԍv��

//------------------------------------------------------------------------------
// Link Libraries
//------------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "AnalysisDrive_Debug.lib") // Debug��
#else
#pragma comment(lib, "AnalysisDrive.lib") // Release��
#endif

#endif // ANALYSISDRIVE_ANALYSISDRIVE_H_
