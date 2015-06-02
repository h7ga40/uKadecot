using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Codeplex.Data;

namespace KadecotNames
{
	class Program
	{
		delegate int GetDataLenCallBack<T>(CharNode<T> node);
		delegate void SetDataCallBack<T>(CharNode<T> node, byte[] data, int pos);

		static void Main(string[] args)
		{
			if (!File.Exists("KadecotDevices.json")) {
				WebClient wc = new WebClient();
				wc.Proxy = WebRequest.GetSystemWebProxy();
				wc.BaseAddress = "http://app.kadecot.net/docs/ProcTopic/echo_devices/";
				MemoryStream devicesJson = new MemoryStream(wc.DownloadData("devices.json"));
				var devices = DynamicJson.Parse(devicesJson);
				List<KadecotDevice> devList = new List<KadecotDevice>();

				foreach (var dev in devices.data) {
					KadecotDevice kd = new KadecotDevice(dev);
					devList.Add(kd);
				}

				foreach (var dev in devList) {
					MemoryStream deviceJson = new MemoryStream(wc.DownloadData(dev.DeviceName + ".json"));
					var device = DynamicJson.Parse(deviceJson);
					foreach (var prop in device.methods) {
						KadecotProperty kp = new KadecotProperty(prop);
						dev.Properties.Add(kp);
					}
				}

				string result = DynamicJson.Serialize(devList);
				result = FormatJson(result);
				result = result.Replace(":undefined,", ":\"undefined\",");
				result = result.Replace(":mandatory,", ":\"mandatory\",");
				result = result.Replace(":optional,", ":\"optional\",");
				using (FileStream fs = new FileStream("KadecotDevices.json", FileMode.Create, FileAccess.Write)) {
					byte[] data = Encoding.UTF8.GetBytes(result);
					fs.Write(data, 0, data.Length);
				}
			}

			if (File.Exists("KadecotDevices.json")) {
				dynamic json;
				var devices = new SortedDictionary<string, KadecotDevice>();
				var properties = new SortedDictionary<string, SortedList<byte, List<KadecotProperty>>>();

				using (FileStream fs = new FileStream("KadecotDevices.json", FileMode.Open, FileAccess.Read)) {
					json = DynamicJson.Parse(fs);
				}

				foreach (var dev in json) {
					var device = new KadecotDevice(dev, true);
					devices.Add(device.DeviceName + "\0", device);

					foreach (var prop in dev.Properties) {
						var property = new KadecotProperty(device, prop);

						device.Properties.Add(property);

						SortedList<byte, List<KadecotProperty>> props;
						if (properties.TryGetValue(property.Name + "\0", out props)) {
							List<KadecotProperty> list;
							if (props.TryGetValue(property.Epc, out list)) {
								list.Add(property);
							}
							else {
								list = new List<KadecotProperty>();
								list.Add(property);
								props.Add(property.Epc, list);
							}
						}
						else {
							props = new SortedList<byte, List<KadecotProperty>>();
							var list = new List<KadecotProperty>();
							list.Add(property);
							props.Add(property.Epc, list);
							properties.Add(property.Name + "\0", props);
						}
					}
				}

				var devTree = new CharNode<KadecotDevice>('\0');
				foreach (var dev in devices) {
					AddNode(devTree, 0, dev);
				}

				ReduceLink(devTree);

				int len = SetPosition(devTree, 0, (p) => { return 2; });
				byte[] devTable = new byte[len];
				SetData(devTree, devTable, (node, _data, pos) =>
				{
					_data[pos++] = (byte)(node.Value.DeviceType >> 8);
					_data[pos++] = (byte)(node.Value.DeviceType & 0xFF);
				});

				using (FileStream fs = new FileStream("KadecotNames.bin", FileMode.Create, FileAccess.Write)) {
					byte[] hdr = new byte[2];
					hdr[0] = (byte)(len >> 8);
					hdr[1] = (byte)(len & 0xFF);
					fs.Write(hdr, 0, hdr.Length);
					fs.Write(devTable, 0, devTable.Length);
				}

				var devCodeTree = new CharNode<List<ushort>>();

				foreach (var dev in devices.Values) {
					char[] name = (dev.DeviceName + "\0").ToCharArray();
					List<ushort> pankuzu = new List<ushort>();
					ushort devCode = get_device_type(devTable, devTable.Length, name, name.Length - 1, pankuzu);
					if (dev.DeviceType != devCode)
						System.Diagnostics.Debugger.Break();

					StringBuilder devType = new StringBuilder();
					for (int i = 0x8000; i > 0; i >>= 1) {
						devType.Append(((devCode & i) != 0) ? '1' : '0');
					}

					AddNode(devCodeTree, 0, new KeyValuePair<string, List<ushort>>(devType.ToString(), pankuzu));
				}

				ReduceLink(devCodeTree);

				len = SetPosUShort(devCodeTree, 0, (p) => { return 1 + 2 * p.Value.Count; });
				byte[] devCodeTable = new byte[len];
				devCodeTable[0] = (byte)devCodeTree.Chara.Count;
				SetDataUShort(devCodeTree, 0, devCodeTable, (node, _data, pos) =>
				{
					_data[pos++] = (byte)(node.Value.Count);
					foreach (var devNamePos in node.Value) {
						_data[pos++] = (byte)(devNamePos >> 8);
						_data[pos++] = (byte)(devNamePos & 0xFF);
					}
				});

				using (FileStream fs = new FileStream("KadecotNames.bin", FileMode.Open, FileAccess.Write)) {
					fs.Position = fs.Length;
					byte[] hdr = new byte[2];
					hdr[0] = (byte)(len >> 8);
					hdr[1] = (byte)(len & 0xFF);
					fs.Write(hdr, 0, hdr.Length);
					fs.Write(devCodeTable, 0, devCodeTable.Length);
				}

				foreach (var dev in devices.Values) {
					char[] buf = new char[256];
					if (!get_device_type_name(devCodeTable, devCodeTable.Length, dev.DeviceType, devTable, devTable.Length, buf, buf.Length))
						System.Diagnostics.Debugger.Break();

					int nulchr = 0;
					foreach (var c in buf) {
						nulchr++;
						if (c == '\0')
							break;
					}
					string text = new String(buf, 0, nulchr);
					if (text != (dev.DeviceName + "\0"))
						System.Diagnostics.Debugger.Break();
				}

				var propTree = new CharNode<SortedList<byte, List<KadecotProperty>>>('\0');
				foreach (var prop in properties) {
					AddNode(propTree, 0, prop);
				}

				ReduceLink(propTree);

				len = SetPosition(propTree, 0, PropertyTableDataLen);
				byte[] propTable = new byte[len];
				SetData(propTree, propTable, PropertyTableData);

				using (FileStream fs = new FileStream("KadecotNames.bin", FileMode.Open, FileAccess.Write)) {
					fs.Position = fs.Length;
					byte[] hdr = new byte[2];
					hdr[0] = (byte)(len >> 8);
					hdr[1] = (byte)(len & 0xFF);
					fs.Write(hdr, 0, hdr.Length);
					fs.Write(propTable, 0, propTable.Length);
				}

				var epcTree = new CharNode<List<ushort>>();

				foreach (var dev in devices.Values) {
					foreach (var prop in dev.Properties) {
						char[] name = (prop.Name + "\0").ToCharArray();
						List<ushort> pankuzu = new List<ushort>();
						byte epc = get_property_code(propTable, propTable.Length, name, name.Length - 1, dev.DeviceType, pankuzu);
						if (prop.Epc != epc)
							System.Diagnostics.Debugger.Break();

						StringBuilder devType = new StringBuilder();
						for (int i = 0x80; i > 0; i >>= 1) {
							devType.Append(((epc & i) != 0) ? '1' : '0');
						}

						AddNode(epcTree, 0, new KeyValuePair<string, List<ushort>>(devType.ToString(), pankuzu));
					}
				}

				ReduceLink(epcTree);

				len = SetPosByte(epcTree, 0, (p) => { return 1 + 2 * p.Value.Count; });
				byte[] epcTable = new byte[len];
				epcTable[0] = (byte)epcTree.Chara.Count;
				SetDataByte(epcTree, 0, epcTable, (node, _data, pos) =>
				{
					_data[pos++] = (byte)(node.Value.Count);
					foreach (var devNamePos in node.Value) {
						_data[pos++] = (byte)(devNamePos >> 8);
						_data[pos++] = (byte)(devNamePos & 0xFF);
					}
				});
			}
		}

		private const string INDENT_STRING = "\t";

		static string FormatJson(string json)
		{
			int indentation = 0;
			int quoteCount = 0;
			var result =
				from ch in json
				let quotes = ch == '"' ? quoteCount++ : quoteCount
				let lineBreak = ch == ',' && quotes % 2 == 0 ? ch + Environment.NewLine + String.Concat(Enumerable.Repeat(INDENT_STRING, indentation)) : null
				let openChar = (ch == '{' || ch == '[') && quotes % 2 == 0 ? ch + Environment.NewLine + String.Concat(Enumerable.Repeat(INDENT_STRING, ++indentation)) : ch.ToString()
				let closeChar = (ch == '}' || ch == ']') && quotes % 2 == 0 ? Environment.NewLine + String.Concat(Enumerable.Repeat(INDENT_STRING, (--indentation < 0) ? 0 : indentation)) + ch : ch.ToString()
				select (lineBreak == null) ? ((openChar.Length > 1) ? openChar : closeChar) : lineBreak;

			return String.Concat(result);
		}

		private static void AddNode<T>(CharNode<T> parent, int pos, KeyValuePair<string, T> dev)
		{
			if (dev.Key.Length <= pos) {
				parent.Value = dev.Value;
				return;
			}

			var item = parent.Children;
			char c = dev.Key[pos];
			var node = item.FirstOrDefault((n) => { return n.Chara[0] == c; });
			if (node != null) {
				AddNode(node, pos + 1, dev);
			}
			else {
				node = new CharNode<T>(dev.Key[pos]);
				item.Add(node);
				AddNode(node, pos + 1, dev);
			}
		}

		private static void ReduceLink<T>(CharNode<T> parent)
		{
			foreach (var node in parent.Children) {
				ReduceLink(node);
			}

			if (parent.Children.Count != 1)
				return;

			var child = parent.Children[0];
			parent.Children.Clear();
			parent.Children.AddRange(child.Children);
			parent.Chara.AddRange(child.Chara);
			parent.Value = child.Value;
		}

		private static int SetPosition<T>(CharNode<T> parent, int pos, GetDataLenCallBack<T> cb)
		{
			parent.Position = pos;

			pos += 1/*branch count*/;

			// 末端のノード
			if (parent.Children.Count == 0) {
				/* data */
				pos += cb(parent);
				return pos;
			}

			// 分岐テーブル
			foreach (var node in parent.Children) {
				pos += 1/*string length*/ + node.Chara.Count + 2/*nextpos*/;
			}

			foreach (var node in parent.Children) {
				pos = SetPosition(node, pos, cb);
			}

			return pos;
		}

		private static void SetData<T>(CharNode<T> parent, byte[] data, SetDataCallBack<T> cb)
		{
			int pos = parent.Position;
			byte count = (byte)parent.Children.Count;

			// branch count
			data[pos++] = count;

			// 末端のノード
			if (count == 0) {
				cb(parent, data, pos);
				return;
			}

			foreach (var node in parent.Children) {
				// string length
				data[pos++] = (byte)node.Chara.Count;
				foreach (var c in node.Chara) {
					data[pos++] = (byte)c;
				}
				// nextpos
				data[pos++] = (byte)((node.Position >> 8) & 0xFF);
				data[pos++] = (byte)(node.Position & 0xFF);

				SetData(node, data, cb);
			}
		}

		private static int PropertyTableDataLen(CharNode<SortedList<byte, List<KadecotProperty>>> parent)
		{
			if (parent.Value.Count == 1)
				return 2;

			int count = 0;
			var list = new List<List<KadecotProperty>>();
			foreach (var tmp in parent.Value.Values) {
				count += tmp.Count;
				list.Add(tmp);
			}
			list.Sort((p1, p2) => { return p1.Count - p2.Count; });
			List<KadecotProperty> max = list[list.Count - 1];
			list.RemoveAt(list.Count - 1);

			// 最大個数のプロパティ名称の一群はそれ以外として省く。
			count -= max.Count;

			return 2 * count/*devTypes*/ + (1/*count*/ + 1/*EPC*/) * list.Count + 1/*count=0*/+ 1/*その他のEPC*/;
		}

		private static void PropertyTableData(CharNode<SortedList<byte, List<KadecotProperty>>> parent, byte[] _data, int pos)
		{
			if (parent.Value.Count == 1) {
				_data[pos++] = 0;
				_data[pos++] = parent.Value.Values[0][0].Epc;
				return;
			}

			var list = new List<List<KadecotProperty>>(parent.Value.Values);
			list.Sort((p1, p2) => { return p1.Count - p2.Count; });
			List<KadecotProperty> max = list[list.Count - 1];
			list.RemoveAt(list.Count - 1);

			foreach (var tmp in list) {
				_data[pos++] = (byte)tmp.Count;
				foreach (var p in tmp) {
					_data[pos++] = (byte)(p.Owner.DeviceType >> 8);
					_data[pos++] = (byte)(p.Owner.DeviceType & 0xFF);
				}
				_data[pos++] = tmp[0].Epc;
			}
			_data[pos++] = 0;
			_data[pos++] = max[0].Epc;
		}

		private static int SetPosByte<T>(CharNode<T> node, int pos, GetDataLenCallBack<T> cb)
		{
			node.Position = pos;

			pos += 1/* bit mask */ + 1/* bit pattern */ + 2/* unmatchpos */;

			/* data */
			if (node.Children.Count == 0)
				pos += cb(node);

			foreach (var child in node.Children) {
				pos = SetPosByte(child, pos, cb);
			}

			node.Next = pos;

			return pos;
		}

		private static void SetDataByte<T>(CharNode<T> node, int bitPos, byte[] data, SetDataCallBack<T> cb)
		{
			int pos = node.Position;
			int unmatchpos;

			unmatchpos = node.Next;

			byte mask = 0, bits = 0;
			for (byte i = 0; i < node.Chara.Count; i++) {
				mask |= (byte)(0x8000 >> (bitPos + i));
				if (node.Chara[i] == '1')
					bits |= (byte)(0x8000 >> (bitPos + i));
			}
			// bit mask
			data[pos++] = mask;
			// bit pattern
			data[pos++] = bits;
			// unmatchpos
			data[pos++] = (byte)((unmatchpos >> 8) & 0xFF);
			data[pos++] = (byte)(unmatchpos & 0xFF);

			if (node.Children.Count == 0) {
				// data
				cb(node, data, pos);
				return;
			}

			// 次の分岐を処理
			bitPos += node.Chara.Count;
			foreach (var child in node.Children) {
				SetDataByte(child, bitPos, data, cb);
			}
		}

		private static int SetPosUShort<T>(CharNode<T> node, int pos, GetDataLenCallBack<T> cb)
		{
			node.Position = pos;

			pos += 2/* bit mask */ + 2/* bit pattern */ + 2/* unmatchpos */;

			/* data */
			if (node.Children.Count == 0)
				pos += cb(node);

			foreach (var child in node.Children) {
				pos = SetPosUShort(child, pos, cb);
			}

			node.Next = pos;

			return pos;
		}

		private static void SetDataUShort<T>(CharNode<T> node, int bitPos, byte[] data, SetDataCallBack<T> cb)
		{
			int pos = node.Position;
			int unmatchpos;

			unmatchpos = node.Next;

			ushort mask = 0, bits = 0;
			for (ushort i = 0; i < node.Chara.Count; i++) {
				mask |= (ushort)(0x8000 >> (bitPos + i));
				if (node.Chara[i] == '1')
					bits |= (ushort)(0x8000 >> (bitPos + i));
			}
			// bit mask
			data[pos++] = (byte)(mask >> 8);
			data[pos++] = (byte)(mask & 0xFF);
			// bit pattern
			data[pos++] = (byte)(bits >> 8);
			data[pos++] = (byte)(bits & 0xFF);
			// unmatchpos
			data[pos++] = (byte)((unmatchpos >> 8) & 0xFF);
			data[pos++] = (byte)(unmatchpos & 0xFF);

			if (node.Children.Count == 0) {
				// data
				cb(node, data, pos);
				return;
			}

			// 次の分岐を処理
			bitPos += node.Chara.Count;
			foreach (var child in node.Children) {
				SetDataUShort(child, bitPos, data, cb);
			}
		}

		static ushort get_device_type(byte[] table, int len, char[] name, int nameLen, List<ushort> pankuzu)
		{
			int pos = 0, nextpos, i = 0;
			byte count;
			ushort DeviceType;

			while (pos < len) {
				/* branch count */
				count = table[pos++];

				/* 末端のノード */
				if (count == 0) {
					/* NULL文字も含めたサイズ */
					if (i != (nameLen + 1))
						return 0;

					DeviceType = (ushort)(table[pos++] << 8);
					DeviceType |= table[pos++];

					return DeviceType;
				}

				for (; ; ) {
					int length, start = pos, j, k;
					/* string length */
					length = table[pos++];
					for (j = 0, k = i; j < length; j++, k++) {
						/* NULL文字も含めたチェック */
						if ((k > nameLen) || (table[pos++] != name[k])) {
							pos = start + 1 + length + 2;
							goto nextnode;
						}
					}
					i += length;

					/* nextpos */
					nextpos = table[pos++] << 8;
					nextpos |= table[pos++];

					pos = nextpos;
					pankuzu.Add((ushort)start);
					break;
				nextnode:
					count--;
					if (count == 0)
						return 0;
					continue;
				}
			}

			return 0;
		}

		static bool get_device_type_name(byte[] table, int len, ushort deviceType, byte[] strtable, int stablelen, char[] buf, int bufLen)
		{
			int pos = 0, unmatchpos;

			while (pos < len) {
				ushort mask, ptrn;
				/* bit mask */
				mask = (ushort)(table[pos++] << 8);
				mask |= table[pos++];
				/* bit pattarn */
				ptrn = (ushort)(table[pos++] << 8);
				ptrn |= table[pos++];
				/* unmatchpos */
				unmatchpos = table[pos++] << 8;
				unmatchpos |= table[pos++];

				if ((mask & deviceType) != ptrn) {
					if (unmatchpos == 0)
						return false;
					pos = unmatchpos;
					continue;
				}

				/* 末端のノード */
				if ((mask & 0x0001) != 0) {
					int bufpos = 0;
					int count = table[pos++];
					for (int j = 0; j < count; j++) {
						int substr;
						substr = (ushort)(table[pos++] << 8);
						substr |= table[pos++];

						if (substr > stablelen)
							return false;

						int strlen = strtable[substr++];
						if ((substr + strlen) > stablelen)
							return false;

						int end = bufpos + strlen;
						if (end > bufLen)
							return false;

						for (; bufpos < end; bufpos++) {
							buf[bufpos] = (char)strtable[substr++];
						}
					}

					return true;
				}
			}

			return false;
		}

		static byte get_property_code(byte[] table, int len, char[] name,
			int nameLen, ushort devType, List<ushort> pankuzu)
		{
			int pos = 0, nextpos, i = 0;
			byte count;
			byte epc;

			while (pos < len) {
				/* branch count */
				count = table[pos++];

				/* 末端のノード */
				if (count == 0) {
					/* NULL文字も含めたサイズ */
					if (i != (nameLen + 1))
						return 0;

					epc = get_prop_code(table, len, name, nameLen, devType, pos);
					return epc;
				}

				for (; ; ) {
					int length, start = pos, j, k;
					/* string length */
					length = table[pos++];
					for (j = 0, k = i; j < length; j++, k++) {
						/* NULL文字も含めたチェック */
						if ((k > nameLen) || (table[pos++] != name[k])) {
							pos = start + 1 + length + 2;
							goto nextnode;
						}
					}
					i += length;

					/* nextpos */
					nextpos = table[pos++] << 8;
					nextpos |= table[pos++];

					pos = nextpos;
					pankuzu.Add((ushort)start);
					break;
				nextnode:
					count--;
					if (count == 0)
						return 0;
					continue;
				}
			}

			return 0;
		}

		static byte get_prop_code(byte[] table, int len, char[] name,
			int nameLen, ushort devType, int pos)
		{
			int i, start;
			byte count;
			byte epc;
			ushort type;

			while (pos < len) {
				/* branch count */
				count = table[pos++];

				/* 末端のノード */
				if (count == 0) {
					epc = table[pos++];
					return epc;
				}

				start = pos;
				for (i = 0; i < count; i++) {
					type = (ushort)(table[pos++] << 8);
					type |= table[pos++];
					if (type == devType) {
						epc = table[start + 2 * count];
						return epc;
					}
				}
				pos++;
			}

			epc = table[pos];
			return epc;
		}
	}

	public class KadecotDevice
	{
		private string m_Protocol;
		private ushort m_DeviceType;
		private string m_DeviceName;
		private List<KadecotProperty> m_Properties = new List<KadecotProperty>();

		public KadecotDevice(dynamic json)
		{
			m_Protocol = json.protocol;
			m_DeviceType = UInt16.Parse(
				((string)json.deviceType).Substring(2, 4),
				System.Globalization.NumberStyles.AllowHexSpecifier);
			m_DeviceName = json.deviceName;
		}

		public KadecotDevice(dynamic json, bool dummy)
		{
			m_Protocol = json.Protocol;
			m_DeviceType = (ushort)json.DeviceType;
			m_DeviceName = json.DeviceName;
		}

		public string Protocol
		{
			get { return m_Protocol; }
			set { m_Protocol = value; }
		}

		public ushort DeviceType
		{
			get { return m_DeviceType; }
			set { m_DeviceType = value; }
		}

		public string DeviceName
		{
			get { return m_DeviceName; }
			set { m_DeviceName = value; }
		}

		public List<KadecotProperty> Properties
		{
			get { return m_Properties; }
		}
	}

	public enum PropertyAttribute
	{
		undefined,
		mandatory,
		optional,
	}

	public class KadecotProperty
	{
		private KadecotDevice m_Owner;
		private string m_Name;
		private byte m_Epc;
		private byte[] m_Size;
		private byte m_MaxSize;
		private PropertyAttribute m_Announce;
		private PropertyAttribute m_Set;
		private PropertyAttribute m_Get;
		private string m_Document;
		private string m_SizeDisp;

		public KadecotProperty(dynamic json)
		{
			m_Name = Regex.Replace((string)json.name, "([^0-9A-Za-z_]+)", "");
			m_Epc = Byte.Parse(((string)json.epc).Substring(2, 2),
				System.Globalization.NumberStyles.AllowHexSpecifier);
			var temp1 = (string)json.size;
			if (temp1.StartsWith("Max")) {
				Byte.TryParse(temp1.Substring(3, temp1.Length - 3), out m_MaxSize);
			}
			else {
				var temp = (temp1).Split(new[] { "or" }, StringSplitOptions.RemoveEmptyEntries);
				var temp2 = new List<byte>();
				foreach (var p in temp) {
					byte size;
					if (Byte.TryParse(p, out size)) {
						temp2.Add(size);
					}
				}
				m_Size = temp2.ToArray();
			}
			m_Announce = GetAttribute((string)json.announce);
			m_Set = GetAttribute((string)json.set);
			m_Get = GetAttribute((string)json.get);
			m_Document = json.doc;
		}

		public KadecotProperty(KadecotDevice owner, dynamic json)
		{
			m_Owner = owner;
			m_Name = json.Name;
			m_Epc = (byte)json.Epc;
			m_Size = (byte[])json.Size;
			m_MaxSize = (byte)json.MaxSize;
			m_Announce = GetAttribute((string)json.Announce);
			m_Set = GetAttribute((string)json.Set);
			m_Get = GetAttribute((string)json.Get);
			m_Document = json.Document;

			if (m_Size == null) {
				m_SizeDisp = "Max" + m_MaxSize;
			}
			else if (m_Size.Length == 1) {
				m_SizeDisp = m_Size[0].ToString();
			}
			else if (m_Size.Length == 1) {
				List<string> temp = new List<string>();
				foreach (var size in m_Size) {
					temp.Add(size.ToString());
				}
				m_SizeDisp = "Max" + String.Join("or", temp);
			}
		}

		public KadecotDevice Owner
		{
			get { return m_Owner; }
		}

		public string Name
		{
			get { return m_Name; }
		}

		public byte Epc
		{
			get { return m_Epc; }
		}

		public byte[] Size
		{
			get { return m_Size; }
		}

		public byte MaxSize
		{
			get { return m_MaxSize; }
		}

		public PropertyAttribute Announce
		{
			get { return m_Announce; }
		}

		public PropertyAttribute Set
		{
			get { return m_Set; }
		}

		public PropertyAttribute Get
		{
			get { return m_Get; }
		}

		public string Document
		{
			get { return m_Document; }
		}

		public static PropertyAttribute GetAttribute(string p)
		{
			switch (p.ToLower()) {
			case "undefined":
				return PropertyAttribute.undefined;
			case "mandatory":
				return PropertyAttribute.mandatory;
			case "optional":
				return PropertyAttribute.optional;
			default:
				throw new Exception();
			}
		}

		public string SizeDisp { get { return m_SizeDisp; } }
	}

	public class CharNode<T>
	{
		public List<char> Chara = new List<char>();
		public List<CharNode<T>> Children = new List<CharNode<T>>();
		public T Value;
		public int Position;
		public int Next;

		public CharNode()
		{
		}

		public CharNode(char chara)
		{
			Chara.Add(chara);
		}

		public override string ToString()
		{
			var chars = new List<string>();
			foreach (var child in Children) {
				chars.Add(new String(child.Chara.ToArray()));
			}

			return String.Format("{0}{{{1}}}", Chara, String.Join(", ", chars));
		}
	}
}
