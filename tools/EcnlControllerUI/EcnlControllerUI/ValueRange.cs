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
using System.Text.RegularExpressions;

namespace ctrlui
{
	enum TokenType
	{
		Separetor,
		HexValue,
		Numeric,
		String,
		End,
	};

	class TokenInfo
	{
		public string Token;
		public TokenType Type;
		public static string Separetor = @"[=,:\(\)\[\]～]";
		public static string HexValue = "^0x[0-9A-Fa-f]+";
		public static string Numeric = @"^[\-\+]?[\.]?[0-9]+[0-9,\.]*";

		public TokenInfo(string token, TokenType type)
		{
			Token = token;
			Type = type;
		}

		public override string ToString()
		{
			return Token;
		}

		public int ToInt32()
		{
			switch (Type) {
			case TokenType.HexValue:
				return Int32.Parse(Token.Substring(2)/*0xの削除*/, 16);
			case TokenType.Numeric:
				return Int32.Parse(Token);
			default:
				return 0;
			}
		}

		public long ToInt64(string typeName)
		{
			string temp;

			switch (Type) {
			case TokenType.HexValue:
				temp = Token.Substring(2)/*0xの削除*/;
				return Int32.Parse(temp, 16);
			case TokenType.Numeric:
				return Int32.Parse(Token);
			default:
				return 0;
			}
		}
	}

	enum State
	{
		Value,
		EqualOrRange,
		Description,
		UnitOrCommaOrEnd,
		RengeMax,
		StartParenthesis,
		RangeDescription,
		MinDescription,
		RangeSeparetor,
		MaxDescription,
		EndParenthesis,
		CommaOrEnd,
		BitFieldLSB,
		BitRange,
		BitFieldMSB,
		BitFieldDescription,
		BitRangeEnd,
		Colon,
	}

	public abstract class Value
	{
		public abstract string Disp { get; }
		public abstract string GetInitialValue();
	}

	public class Option : Value
	{
		private long _Val;
		private string _Disp;

		public Option(long val, string disp)
		{
			_Val = val;
			_Disp = disp;
		}

		public long Val { get { return _Val; } }
		public override string Disp { get { return _Disp; } }

		public override string GetInitialValue()
		{
			return _Val.ToString(16).ToUpper();
		}
	}

	public class Range : Value
	{
		private long _Min;
		private long _Max;
		private string _Disp;
		private string _MinDisp;
		private string _MaxDisp;
		private string _Unit;
		private long _InitialValue;

		public Range(long min, long max, string disp)
		{
			_Min = min;
			_Max = max;
			_Disp = disp;
			_MinDisp = "";
			_MaxDisp = "";
			_Unit = "";
			_InitialValue = min;
		}

		public Range(long min, long max, string disp, string minDisp, string maxDisp, string unit)
		{
			_Min = min;
			_Max = max;
			_Disp = disp;
			_MinDisp = minDisp;
			_MaxDisp = maxDisp;
			_Unit = unit;
			_InitialValue = min;
		}

		public long Min { get { return _Min; } }
		public long Max { get { return _Max; } }
		public override string Disp { get { return _Disp; } }
		public string MinDisp { get { return _MinDisp; } }
		public string MaxDisp { get { return _MaxDisp; } }
		public string Unit { get { return _Unit; } }
		public long InitialValue
		{
			get { return _InitialValue; }
			set { _InitialValue = value; }
		}

		public override string GetInitialValue()
		{
			return _InitialValue.ToString(16).ToUpper();
		}

		public string GetDisp(long value)
		{
			return _Disp + value.ToString() + _Unit;
		}
	}

	public class InRangeValue : Value
	{
		private Range _Range;
		private long _Value;

		public InRangeValue(Range range, long value)
		{
			_Range = range;
			_Value = value;
		}

		public Range Range { get { return _Range; } }
		public long Value { get { return _Value; } }
		public override string Disp { get { return _Range.GetDisp(_Value); } }

		public override string GetInitialValue()
		{
			return _Value.ToString(16).ToUpper();
		}
	}

	public class BitField : Value
	{
		private int _Least;
		private int _Most;
		private string _Disp;
		private List<Option> _Values = new List<Option>();
		private List<Range> _Ranges = new List<Range>();
		private Value _InitailValue;

		public BitField(int least, int most, string disp)
		{
			_Least = least;
			_Most = most;
			_Disp = disp;
		}

		public int Least { get { return _Least; } }
		public int Most { get { return _Most; } }
		public override string Disp { get { return _Disp; } }
		public List<Option> Values { get { return _Values; } }
		public List<Range> Ranges { get { return _Ranges; } }
		public Value InitailValue { get { return _InitailValue; } }

		public override string GetInitialValue()
		{
			if (_InitailValue == null)
				return "";
			return _InitailValue.GetInitialValue();
		}

		internal void AddValue(Option option)
		{
			if (_InitailValue == null)
				_InitailValue = option;
			_Values.Add(option);
		}

		internal void AddRange(Range range)
		{
			if (_InitailValue == null)
				_InitailValue = range;
			_Ranges.Add(range);
		}

		internal bool IsInRange(long val64)
		{
			foreach (var val in _Values) {
				if (val.Val != val64)
					continue;

				return true;
			}

			foreach (var rng in _Ranges) {
				if ((rng.Min > val64) || (rng.Max < val64))
					continue;

				return true;
			}

			return false;
		}

		internal Value GetInRangeValue(long val64)
		{
			foreach (var val in _Values) {
				if (val.Val != val64)
					continue;

				return val;
			}

			foreach (var rng in _Ranges) {
				if ((rng.Min > val64) || (rng.Max < val64))
					continue;

				return new InRangeValue(rng, val64);
			}

			return null;
		}
	}

	public class BitFiledsValue : Value
	{
		private BitField[] _BitFields;
		private Value[] _BitFieldValues;
		private long _Value;

		public BitFiledsValue(BitField[] bitFlds, Value[] bitFldVals, long value)
		{
			_BitFields = bitFlds;
			_BitFieldValues = bitFldVals;
			_Value = value;
		}

		public override string Disp
		{
			get { return ""; }
		}

		public override string GetInitialValue()
		{
			return _Value.ToString(16).ToUpper();
		}
	}

	class ValueSet
	{
		public List<Option> values;
		public List<Range> ranges;
		public BitField bitField;
		public Value initialValue;

		public ValueSet(List<Option> values, List<Range> ranges, BitField bitField, Value initialValue)
		{
			this.values = values;
			this.ranges = ranges;
			this.bitField = bitField;
			this.initialValue = initialValue;
		}
	}

	public class ValueRange
	{
		private List<Option> _Values = new List<Option>();
		private List<Range> _Ranges = new List<Range>();
		private List<BitField> _BitFields = new List<BitField>();
		private List<ValueRange> _MemberFields = new List<ValueRange>();
		private Value _InitailValue;
		static Dictionary<string, ValueSet> _ParsedText = new Dictionary<string, ValueSet>();

		public List<Option> Values { get { return _Values; } }
		public List<Range> Ranges { get { return _Ranges; } }
		public List<BitField> BitFields { get { return _BitFields; } }
		public List<ValueRange> MemberFields { get { return _MemberFields; } }

		public string InitailValue
		{
			get
			{
				if (_InitailValue == null)
					return "";
				return _InitailValue.GetInitialValue();
			}
			set
			{
				int val64 = Int32.Parse(value);
				foreach (var val in _Values) {
					if (val.Val != val64)
						continue;

					_InitailValue = val;
					return;
				}

				foreach (var rng in _Ranges) {
					if ((rng.Min > val64) || (rng.Max < val64))
						continue;

					_InitailValue = new InRangeValue(rng, val64);
					return;
				}

				if (_BitFields.Count > 0) {
					bool ok = true;
					List<Value> bitFlds = new List<Value>();
					foreach (var bit in _BitFields) {
						if (!bit.IsInRange(val64)) {
							ok = false;
							break;
						}

						bitFlds.Add(bit.GetInRangeValue(val64));
					}

					if (ok) {
						_InitailValue = new BitFiledsValue(_BitFields.ToArray(), bitFlds.ToArray(), val64);
						return;
					}
				}
			}
		}

		private static bool ParseValueRange(string input, string type, List<Option> values, List<Range> ranges,
			out BitField bitFeild, out Value initialValue)
		{
			bitFeild = null;
			initialValue = null;

			List<TokenInfo> tokens = new List<TokenInfo>();
			int pos = 0;
			while (pos < input.Length) {
				Regex ms;
				RegexMatch m;
				if ((m = (ms = new Regex(TokenInfo.HexValue)).Exec(input.Substring(pos))) != null) {
					tokens.Add(new TokenInfo(m[0], TokenType.HexValue));
					pos += m[0].Length;
				}
				else if ((m = (ms = new Regex(TokenInfo.Numeric)).Exec(input.Substring(pos))) != null) {
					tokens.Add(new TokenInfo(m[0], TokenType.Numeric));
					pos += m[0].Length;
				}
				else {
					if ((m = (ms = new Regex(TokenInfo.Separetor)).Exec(input.Substring(pos))) == null) {
						tokens.Add(new TokenInfo(input.Substring(pos), TokenType.String));
						break;
					}
					int index = pos + m.Index;
					if (index == pos) {
						tokens.Add(new TokenInfo(m[0], TokenType.Separetor));
						pos += 1;
					}
					else {
						tokens.Add(new TokenInfo(input.Substring(pos, index - pos), TokenType.String));
						tokens.Add(new TokenInfo(m[0], TokenType.Separetor));
						pos = index + 1;
					}
				}
			}
			tokens.Add(new TokenInfo("", TokenType.End));

			bool OK = false;
			List<TokenInfo> sentence = new List<TokenInfo>();
			State state = State.Value;
			foreach (TokenInfo token in tokens) {
				switch (state) {
				case State.Value:
					if ((token.Type == TokenType.HexValue)
						|| (token.Type == TokenType.Numeric)) {
						sentence.Add(token);
						state = State.EqualOrRange;
						continue;
					}
					else if ((token.Type == TokenType.Separetor)
						&& (token.Token == "[")) {
						if (bitFeild != null)
							break;
						state = State.BitFieldLSB;
						continue;
					}
					break;
				case State.EqualOrRange:
					if (token.Type == TokenType.Separetor) {
						switch (token.Token) {
						case "=":
							state = State.Description;
							continue;
						case "～":
							state = State.RengeMax;
							continue;
						}
					}
					else if (token.Type == TokenType.End) {
						Option option = new Option(sentence[0].ToInt64(type), "固定");
						if (bitFeild != null)
							bitFeild.AddValue(option);
						else {
							if (initialValue == null)
								initialValue = option;
							values.Add(option);
						}
						sentence.Clear();
						state = State.CommaOrEnd;
						continue;
					}
					break;
				case State.Description:
					if (token.Type == TokenType.String) {
						Option option = new Option(sentence[0].ToInt64(type), token.Token);
						if (bitFeild != null)
							bitFeild.AddValue(option);
						else {
							if (initialValue == null)
								initialValue = option;
							values.Add(option);
						}
						sentence.Clear();
						state = State.CommaOrEnd;
						continue;
					}
					else if (token.Type == TokenType.Numeric) {
						sentence.Add(token);
						state = State.UnitOrCommaOrEnd;
						continue;
					}
					break;
				case State.UnitOrCommaOrEnd:
					if (token.Type == TokenType.String) {
						Option option = new Option(sentence[0].ToInt64(type), sentence[1].Token + token.Token);
						if (bitFeild != null)
							bitFeild.AddValue(option);
						else {
							if (initialValue == null)
								initialValue = option;
							values.Add(option);
						}
						sentence.Clear();
						state = State.CommaOrEnd;
						continue;
					}
					else if ((token.Type == TokenType.Separetor)
						&& (token.Token == ",")) {
						Option option = new Option(sentence[0].ToInt64(type), sentence[1].Token);
						if (bitFeild != null)
							bitFeild.AddValue(option);
						else {
							if (initialValue == null)
								initialValue = option;
							values.Add(option);
						}
						sentence.Clear();
						state = State.Value;
						continue;
					}
					else if (token.Type == TokenType.End) {
						Option option = new Option(sentence[0].ToInt64(type), sentence[1].Token);
						if (bitFeild != null)
							bitFeild.AddValue(option);
						else {
							if (initialValue == null)
								initialValue = option;
							values.Add(option);
						}
						sentence.Clear();
						OK = true;
					}
					break;
				case State.RengeMax:
					if ((token.Type == TokenType.HexValue)
						|| (token.Type == TokenType.Numeric)) {
						sentence.Add(token);
						state = State.StartParenthesis;
						continue;
					}
					else if ((token.Type == TokenType.Separetor)
						&& (token.Token == "=")) {
						string max = "0x";
						for (int i = sentence[0].Token.Length - 2; i > 0; i--) {
							max += "F";
						}
						sentence.Add(new TokenInfo(max, TokenType.HexValue));
						state = State.RangeDescription;
						continue;
					}
					break;
				case State.StartParenthesis:
					if (token.Type == TokenType.Separetor) {
						switch (token.Token) {
						case "=":
							if (sentence.Count == 2) {
								state = State.RangeDescription;
								continue;
							}
							break;
						case "(":
							sentence.Add(new TokenInfo("", TokenType.String));
							state = State.MinDescription;
							continue;
						case ",":
							Range range = new Range(sentence[0].ToInt64(type), sentence[1].ToInt64(type), (sentence.Count > 2) ? sentence[2].Token : "");
							if (bitFeild != null) {
								bitFeild.AddRange(range);
							}
							else {
								if (initialValue == null)
									initialValue = range;
								ranges.Add(range);
							}
							sentence.Clear();
							state = State.Value;
							continue;
						}
					}
					else if (token.Type == TokenType.End) {
						Range range = new Range(sentence[0].ToInt64(type), sentence[1].ToInt64(type), (sentence.Count > 2) ? sentence[2].Token : "");
						if (bitFeild != null) {
							bitFeild.AddRange(range);
						}
						else {
							if (initialValue == null)
								initialValue = range;
							ranges.Add(range);
						}
						sentence.Clear();
						OK = true;
					}
					break;
				case State.RangeDescription:
					if (token.Type == TokenType.String) {
						sentence.Add(token);
						state = State.StartParenthesis;
						continue;
					}
					break;
				case State.MinDescription:
					if ((token.Type == TokenType.Numeric)
						|| (token.Type == TokenType.String)) {
						sentence.Add(token);
						state = State.RangeSeparetor;
						continue;
					}
					break;
				case State.RangeSeparetor:
					if ((token.Type == TokenType.Separetor)
						&& (token.Token == "～")) {
						state = State.MaxDescription;
						continue;
					}
					break;
				case State.MaxDescription:
					if ((token.Type == TokenType.Numeric)
						|| (token.Type == TokenType.String)) {
						sentence.Add(token);
						state = State.EndParenthesis;
						continue;
					}
					break;
				case State.EndParenthesis:
					if ((token.Type == TokenType.Separetor)
						&& (token.Token == ")")) {
						Range range = new Range(
							sentence[0].ToInt64(type),
							sentence[1].ToInt64(type),
							sentence[2].Token,
							sentence[3].Token,
							sentence[4].Token,
							(sentence.Count > 5) ? sentence[5].Token : "");
						if (bitFeild != null) {
							bitFeild.AddRange(range);
						}
						else {
							if (initialValue == null)
								initialValue = range;
							ranges.Add(range);
						}
						sentence.Clear();
						state = State.CommaOrEnd;
						continue;
					}
					else if (token.Type == TokenType.String) {
						sentence.Add(token);
						state = State.EndParenthesis;
						continue;
					}
					break;
				case State.BitFieldLSB:
					if (token.Type == TokenType.Numeric) {
						sentence.Add(token);
						state = State.BitRange;
						continue;
					}
					break;
				case State.BitRange:
					if (token.Type == TokenType.Separetor) {
						switch (token.Token) {
						case "～":
							state = State.BitFieldMSB;
							continue;
						case "]":
							sentence.Add(sentence[0]);
							state = State.BitFieldDescription;
							continue;
						}
					}
					break;
				case State.BitFieldMSB:
					if (token.Type == TokenType.Numeric) {
						sentence.Add(token);
						state = State.BitRangeEnd;
						continue;
					}
					break;
				case State.BitRangeEnd:
					if ((token.Type == TokenType.Separetor)
						&& (token.Token == "]")) {
						state = State.BitFieldDescription;
						continue;
					}
					break;
				case State.BitFieldDescription:
					if (token.Type == TokenType.String) {
						sentence.Add(token);
						state = State.Colon;
						continue;
					}
					break;
				case State.Colon:
					if ((token.Type == TokenType.Separetor)
						&& (token.Token == ":")) {
						bitFeild = new BitField(sentence[0].ToInt32(), sentence[1].ToInt32(), sentence[2].Token);
						if (initialValue == null)
							initialValue = bitFeild;
						sentence.Clear();
						state = State.Value;
						continue;
					}
					else if (token.Type == TokenType.End) {
						bitFeild = new BitField(sentence[0].ToInt32(), sentence[1].ToInt32(), sentence[2].Token);
						if (initialValue == null)
							initialValue = bitFeild;
						sentence.Clear();
						OK = true;
						break;
					}
					break;
				case State.CommaOrEnd:
					if (token.Type == TokenType.End) {
						OK = true;
					}
					else if ((token.Type == TokenType.Separetor)
						&& (token.Token == ",")) {
						state = State.Value;
						continue;
					}
					break;
				}
				break;
			}

			return OK;
		}

		public static ValueRange Parse(string valrng, JsonFieldInfo emti)
		{
			ValueRange valueRange = new ValueRange();

			if (emti.primitive) {
				string[] lines = valrng.Split(new Regex("\r\n"));
				foreach (string line in lines) {
					if (new Regex("^<(.+)>$").Test(line))
						continue;

					List<Option> values;
					List<Range> ranges;
					BitField bitField;
					Value initialValue;

					ValueSet pair;
					if (_ParsedText.TryGetValue(line, out pair)) {
						values = pair.values;
						ranges = pair.ranges;
						bitField = pair.bitField;
						initialValue = pair.initialValue;
					}
					else {
						values = new List<Option>();
						ranges = new List<Range>();

						ParseValueRange(line, emti.type, values, ranges, out bitField, out initialValue);

						_ParsedText.Add(line, new ValueSet(values, ranges, bitField, initialValue));
					}

					valueRange._Values.AddRange(values);
					valueRange._Ranges.AddRange(ranges);
					if (bitField != null)
						valueRange._BitFields.Add(bitField);
					if (valueRange._InitailValue == null)
						valueRange._InitailValue = initialValue;
				}
			}
			else {
				foreach (JsonFieldInfo efi in emti.fields) {
					ValueRange member = Parse(efi.valueDescription, efi);
					valueRange._MemberFields.Add(member);
				}
			}

			return valueRange;
		}
	}
}
