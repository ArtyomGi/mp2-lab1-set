// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле



#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) throw "Length should be a positive number";
	BitLen = len;
	MemLen = (BitLen + 31) / 32;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	if (&bf != this)
	{
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{

	return n / 32; // n << 5
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0) throw "Bit number should be positive";
	if (n > BitLen) throw "Bit number should be lower than overall number of bits";
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0) throw "Bit number should be positive";
	if (n > BitLen) throw "Bit number should be lower than overall number of bits";
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) throw "Bit number should be positive";
	if (n > BitLen) throw "Bit number should be lower than overall number of bits";
	unsigned int ret = pMem[GetMemIndex(n)] & GetMemMask(n);
	if (ret) return 1;
	else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if(&bf == this) return *this;
	if (BitLen != bf.BitLen) { // перевыделение памяти
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
		
	}
	
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i]) return 0;
	/*else if (bf.MemLen < MemLen)
	{
		for (int i = 0; i < bf.MemLen; i++)
			if (pMem[i] != bf.pMem[i]) return 0;
		for (int i = bf.MemLen; i < MemLen; i++)
			if (pMem[i]) return 0;
	}
	else if (bf.MemLen > MemLen)
	{
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i]) return 0;
		for (int i = MemLen; i < bf.MemLen; i++)
			if (bf.pMem[i]) return 0;
	}*/
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !operator==(bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int NewLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
	TBitField ret(NewLen);

	if (MemLen > bf.MemLen)
	{
		for (int i = 0; i < bf.MemLen; i++)
			ret.pMem[i] = pMem[i] | bf.pMem[i];
		for(int i = bf.MemLen; i < MemLen; i++)
			ret.pMem[i] = pMem[i];
	}
	else if (MemLen < bf.MemLen)
	{
		for (int i = 0; i < MemLen; i++)
			ret.pMem[i] = pMem[i] | bf.pMem[i];
		for (int i = MemLen; i < bf.MemLen; i++)
			ret.pMem[i] = bf.pMem[i];
	}
	else
	{
		for (int i = 0; i < MemLen; i++)
			ret.pMem[i] = pMem[i] | bf.pMem[i];
	}
	
	return ret;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int NewLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
	TBitField ret(NewLen);

	if (MemLen > bf.MemLen)
	{
		for (int i = 0; i < bf.MemLen; i++)
			ret.pMem[i] = pMem[i] & bf.pMem[i];
		for (int i = bf.MemLen; i < MemLen; i++)
			ret.pMem[i] = 0;
	}
	else if (MemLen < bf.MemLen)
	{
		for (int i = 0; i < MemLen; i++)
			ret.pMem[i] = pMem[i] & bf.pMem[i];
		for (int i = MemLen; i < bf.MemLen; i++)
			ret.pMem[i] = 0;
	}
	else
	{
		for (int i = 0; i < MemLen; i++)
			ret.pMem[i] = pMem[i] & bf.pMem[i];
	}

	return ret;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField ret(BitLen);

	for (int i = 0; i < MemLen - 1; i++)
		ret.pMem[i] = ~pMem[i];

	for (int i = (MemLen - 1)*32; i < BitLen; i++)
	{
		if (GetBit(i)) ret.ClrBit(i);
		else ret.SetBit(i);
	}

	return ret;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	return ostr;
}
