#ifndef _SGGameSpace_h_
#define _SGGameSpace_h_
/********************************************
	class:	Space
	usage:	
********************************************/
	/*** common header ***/
#include "Common.h"
#include "SGForwardDeclarations.h"
	/*** extra headers if needed ***/
#include <unordered_set>
	/*** end header inclusion ***/

namespace SG
{
	//fwds
	//end fwds

	typedef std::unordered_set< spEntity > EntityList;

	class Cell
	{
	private:
		EntityList _entities;
		Rect _bounds;
		EntityList::iterator _curIterator;

	public:
		Cell(const Vector& pos, const Vector& size);
		~Cell();

		void Insert(spEntity e);
		void Insert(Cell& c);
		void Remove(spEntity e);

		spEntity begin();
		spEntity next();
		bool end() const;

		bool empty() const;
		void clear();
		uint count() const;
	};

	typedef std::vector<Cell> Area;

	class Space
	{
	private:
		static InstanceID _idCounter;
		static InstanceID _GenerateID();

		Space(const Space& rhs);
		Space& Space::operator=(const Space& rhs);

		InstanceID _id;
		Entity& _parent;

		Area _area;

		double _width;
		double _height;
		int _cellsX;
		int _cellsY;
		double _cellSizeX;
		double _cellSizeY;

		int	_PosToIndex(const Vector& pos);

	protected:
		Space(Entity& owner, double width, double height, int cellsX, int cellsY);

	public:
		static spSpace Make(Entity& owner, double width, double height, int cellsX, int cellsY) { return spSpace(new Space(owner, width, height, cellsX, cellsY)); }

		virtual ~Space();

		InstanceID	getID() const;
		Entity&		getParentEntity() const;
		Form&		getParentForm() const;
		double		getWidth() const;
		double		getHeight() const;
		int			getCellsX() const;
		int			getCellsY() const;
		Vector		getCellsSize() const;

		bool	empty();
		void	clear();

		bool	Insert(spEntity e, bool child);
		bool	Update(spEntity e, const Vector& oldPos);
		bool	Remove(spEntity e);
		
		Cell&	GetCell(int x, int y);
		shared_ptr<Cell> GetAll();
		
		spEntity	FindFirstByType(const String& type, bool recursive);
		Cell		FindAllByType(const String& type, bool recursive);
		spEntity	FindFirstByAlias(const String& alias, bool recursive);
		Cell		FindAllByAlias(const String& alias, bool recursive);
	};
}

#endif //_SGGameSpace_h_