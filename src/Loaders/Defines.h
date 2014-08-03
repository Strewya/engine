#pragma once
/********************************************
*	class:	Constants
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
#define INVALID_ID 0

	enum class LoadResultFlag
	{
		Success,
		Fail,
		DependencyMissing
	};

	class LoadResult
	{
	public:
		LoadResultFlag flag;
		std::string info;
		
		operator bool() { return flag == LoadResultFlag::Success; }
	};

	inline std::ostream& operator<<(std::ostream& os, const LoadResult& res)
	{
		switch( res.flag )
		{
			case LoadResultFlag::Success:
				os << "Success";
				break;
			case LoadResultFlag::Fail:
				os << "Failure - ";
				break;
			case LoadResultFlag::DependencyMissing:
				os << "Dependency missing - ";
				break;
			default:
				break;
		}
		os << res.info;
		return os;
	}
}
