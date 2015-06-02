/*
 *  TOPPERS ECHONET Lite Communication Middleware
 * 
 *  Copyright (C) 2015 Cores Co., Ltd. Japan
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id$
 */

using System;
using System.Collections.Generic;
using System.Web;

namespace ctrlui
{
	public enum ENOD_ID
	{
		/// <summary>
		/// NOT_MATCH_ID
		/// </summary>
		NOT_MATCH_ID = -1,
		/// <summary>
		/// MULTICAST_ID
		/// </summary>
		MULTICAST_ID = 0,
		/// <summary>
		/// LOCAL_ID
		/// </summary>
		LOCAL_ID = 1,
		/// <summary>
		/// API_ID 
		/// </summary>
		API_ID = 2,
		/// <summary>
		/// REMOTE_ID
		/// </summary>
		REMOTE_ID = 3,
		/// <summary>
		/// TABLE_NUM 
		/// </summary>
		TABLE_NUM = 100,
	}

	[Flags]
	public enum EPC_FLAG
	{
		/// <summary>なし</summary>
		NONE = 0x00,
		/// <summary>アクセスルール Get</summary>
		RULE_SET = 0x01,
		/// <summary>アクセスルール Set</summary>
		RULE_GET = 0x02,
		/// <summary>アクセスルール Anno</summary>
		RULE_ANNO = 0x04,
		/// <summary>状態変化時通知</summary>
		ANNOUNCE = 0x40,
		/// <summary>可変長データ</summary>
		VARIABLE = 0x80,
	}

	public class T_ECN_EOJ
	{
		private byte[] _eoj = new byte[3];

		public T_ECN_EOJ(byte[] src, int offset)
		{
			_eoj[0] = src[offset];
			_eoj[1] = src[offset + 1];
			_eoj[2] = src[offset + 2];
		}

		public byte x1 { get { return _eoj[0]; } set { _eoj[0] = value; } }

		public byte x2 { get { return _eoj[1]; } set { _eoj[1] = value; } }

		public byte x3 { get { return _eoj[2]; } set { _eoj[2] = value; } }

		/// <summary>プロファイル</summary>
		public const byte X1_PROFILE = 0x0E;

		// プロファイルクラスグループ
		/// <summary>ノードプロファイル</summary>
		public const byte X2_NODE_PROFILE = 0xF0;
	}
}
