#pragma once

#include "GUIMisc.h"

#include "..\Skinner.h"

#include <list>
#include <memory>
#include <algorithm>

namespace SimpleGUI
{
	class GUIEventHandler;
	class Skinner;

	class Base
	{
		typedef std::shared_ptr<Base> pBase;
	public:
		//что этой херне требуется, чтобы она работала?
		Base(Base * parent, std::string name = "");
		virtual ~Base();

		void AddChildren(pBase Child);
		void RemoveChildren(pBase Child);
		void RemoveAllChildrens();

		Base * GetParent() const;

		virtual Base * GetControlAt(int mouseX, int mouseY);

		Point GetPosition() const;
		void SetPosition(Point Pos);
		void SetPosition(int x, int y);

		std::string GetName() const;
		void SetName(const std::string & val);

		Point GetSize() const;
		virtual void SetSize(int x, int y);

		bool IsVisible() const;

		void Show();
		void Hide();

		virtual bool IsHit(const Point & point);
		void Render(Skinner * skin);

		virtual bool CanHaveFocus() const;

		virtual bool OnMouseDown(int mb, int x, int y);
		virtual bool OnMouseUp(int mb, int x, int y);
		virtual bool OnMouseMove();
		virtual bool OnMouseEnter();
		virtual bool OnMouseLeave();

		virtual bool OnKeyChar(wchar_t Key);
		virtual bool OnKeyUp(wchar_t Key);
		virtual bool OnKeyDown(wchar_t Key);

		const std::list<pBase> GetChildrenList() const;

	protected:
		virtual void Draw(Skinner * skin);
		void RecursiveDrawing(Skinner * skin);

		Point Position;
		Point Size;

		bool visible;

		Base * Parent;
		std::list<pBase> Children;
		std::string Name;
	};
}