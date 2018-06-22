/*
 * =====================================================================================
 *
 *       Filename:  MouseItemWidget.hpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 00:50:38
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef MOUSEITEMWIDGET_HPP_
#define MOUSEITEMWIDGET_HPP_

#include "ItemWidget.hpp"

class MouseItemWidget : public ItemWidget {
	public:
		MouseItemWidget(Widget *parent) : ItemWidget(0, parent) {}

		void onEvent(const SDL_Event &event);

		void swapItems(ItemWidget &widget);

	private:
		void updatePosition(float x, float y);
};

#endif // MOUSEITEMWIDGET_HPP_