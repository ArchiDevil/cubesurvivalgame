#pragma once

#include "Objects/Base.h"
#include "../GraphicsEngine/ShiftEngine.h"

namespace SimpleGUI
{
	class Base;
	class Button;
	class Textbox;
	class Image;
	class List;
	class TableRow;
	class Text;
	class ValueBox;

	class Skinner
	{
	public:
		Skinner();
		~Skinner();

		void Initialize();

		template<typename T>
		void Draw(T * item);

		void DrawButton(Button * but);
		void DrawTextbox(Textbox * textbox, const std::string & string, bool Focused);
		void DrawImage(Image * image, const ShiftEngine::Sprite & spr);
		void DrawList(List * list);
		void DrawListRow(TableRow * row);
		void DrawLabel(Text * label);
		void DrawValueBox(ValueBox * valueBox);

	protected:
		void SetControlParameters(ShiftEngine::Sprite * spr, Base * control, const Point & pos, const Point & size);
		void LoadResources();

		ShiftEngine::Sprite * ButtonCache;
		ShiftEngine::Sprite * TextboxCache;
		ShiftEngine::Sprite * ListCache;
		ShiftEngine::Sprite * ValueBoxCache;
	};
}
