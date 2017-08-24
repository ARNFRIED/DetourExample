#pragma once

struct CDataStore
{
	int vTable;
	unsigned char* buffer;		//4
	int base;					//8
	int allocated;				//0xC
	int written;				//0x10
	int readed;					//0x14

	CDataStore() :
		vTable{ 0x0088BA8C },
		buffer{ new unsigned char[0x100] },	//TODO: Optimize this
		base{},
		allocated{ 0x100 },
		written{},
		readed{ -1 }
	{}

	~CDataStore()
	{
		delete buffer;
	}
};

class DataStore
{
public:
	DataStore(int opcode)
	{
		Write<int>(opcode);
	}

	DataStore(CDataStore* pdata)
	{
		pointer = pdata;
	}

	~DataStore()
	{

	}

	template<typename T>
	T Read()
	{
		T result = *(T*)(data.buffer + data.readed);
		data.readed += sizeof(T);
		return result;
	}

	template<typename T>
	T ReadFake()
	{
		T result = *(T*)(pointer->buffer + pointer->readed);
		fake_readed += sizeof(T);
		return result;
	}

	template<typename T>
	void Write(T value)
	{
		// if(data->written + sizeof(T) > data->allocated) --- TODO: See the 3rd vfunc with IDA
		*(T*)(data.buffer + data.written) = value;
		data.written += sizeof(T);
		return;
	}

	auto Send()
	{
		data.readed = 0;
		((void(__cdecl*)(CDataStore&))0x005B3CA0)(data);
	}

private:
	CDataStore* pointer;
	CDataStore data;
	int fake_readed;
};