#pragma once

template <typename T, typename I = int>
class CUtlMemory
{
public:

	T& operator[]( int i )
	{
		return m_pMemory[i];
	}

	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <typename T, typename A = CUtlMemory<T>>
class CUtlVector
{
public:

	using CAllocator = A;

	T& operator[]( int i )
	{
		return m_memory[i];
	}

	CAllocator m_memory;
	int m_Size;
	T* m_pElements;
};

class Vector3
{
public:

	float x{};
	float y{};
	float z{};

	Vector3( )
	{
		x = y = z = 0;
	}

	Vector3( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3& operator=( const Vector3& rhs )
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;

		return *this;
	}

	Vector3 operator + ( const Vector3& rhs ) const { return Vector3( x + rhs.x, y + rhs.y, z + rhs.z ); }
	Vector3 operator - ( const Vector3& rhs ) const { return Vector3( x - rhs.x, y - rhs.y, z - rhs.z ); }
	Vector3 operator * ( const float& rhs ) const { return Vector3( x * rhs, y * rhs, z * rhs ); }
	Vector3 operator / ( const float& rhs ) const { return Vector3( x / rhs, y / rhs, z / rhs ); }

	Vector3& operator += ( const Vector3& rhs ) { return *this = *this + rhs; }
	Vector3& operator -= ( const Vector3& rhs ) { return *this = *this - rhs; }
	Vector3& operator *= ( const float& rhs ) { return *this = *this * rhs; }
	Vector3& operator /= ( const float& rhs ) { return *this = *this / rhs; }

	float Length( ) const 
	{ 
		return sqrtf( x * x + y * y + z * z ); 
	}

	Vector3 Normalize( ) const 
	{ 
		return *this * (1 / Length( )); 
	}

	float Distance( const Vector3& rhs ) const {
		return (*this - rhs).Length( ); 
	}
};

struct Vector4
{
	float values[4];

	Vector4( )
	{
		values[0] = values[1] = values[2] = values[3] = 0;
	}

	Vector4( float x, float y, float z, float w )
	{
		values[0] = x;
		values[1] = y;
		values[2] = z;
		values[3] = w;
	}

	const float& operator[]( int index ) const
	{
		return values[index];
	}

	float& operator[]( int index )
	{
		return values[index];
	}

	Vector4& operator+( Vector4 arg )
	{
		values[0] += arg.values[0];
		values[1] += arg.values[1];
		values[2] += arg.values[2];
		values[3] += arg.values[3];

		return *this;
	}

	Vector4& operator-( Vector4 arg )
	{

		values[0] += arg.values[0];
		values[1] += arg.values[1];
		values[2] += arg.values[2];
		values[3] += arg.values[3];

		return *this;
	}
};

struct Matrix3x4
{
	float matrix[12];
};

struct Matrix4x4
{
	Vector4 columns[4];

	Matrix4x4( )
	{
		columns[0] = Vector4( 1, 0, 0, 0 );
		columns[1] = Vector4( 0, 1, 0, 0 );
		columns[2] = Vector4( 0, 0, 1, 0 );
		columns[3] = Vector4( 0, 0, 0, 1 );
	}

	Matrix4x4( Vector4 x, Vector4 y, Vector4 z, Vector4 w )
	{
		columns[0] = x;
		columns[1] = y;
		columns[2] = z;
		columns[3] = w;
	}

	const Vector4& operator[]( int index ) const
	{
		return columns[index];
	}

	Vector4& operator[]( int index )
	{
		return columns[index];
	}
};