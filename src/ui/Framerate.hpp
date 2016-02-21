#ifndef simplat_ui_Framerate_HeaderPlusPlus
#define simplat_ui_Framerate_HeaderPlusPlus

#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>
#include <Magnum/Text/Renderer.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation2D.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Timeline.h>

#include <cfloat>
#include <chrono>
#include <string>

namespace simplat { namespace ui
{
	using Object2D = Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation2D>;
	using Magnum::SceneGraph::Drawable2D;
	struct Framerate final
	: Object2D
	, Drawable2D
	{
		Framerate(Magnum::SceneGraph::DrawableGroup2D &group, Magnum::Timeline const &timeline, Magnum::Text::AbstractFont &font, Magnum::Text::GlyphCache const &glyphs, Object2D *parent)
		: Object2D{parent}
		, Drawable2D{*this, &group}
		, timeline(timeline)
		, font(font)
		, glyphs(glyphs)
		{
			renderer.reserve
			(
				40, //TODO
				Magnum::BufferUsage::DynamicDraw,
				Magnum::BufferUsage::StaticDraw
			);
			shader
				.setColor(Magnum::Color4{1.0f, 0.0f})
				.setSmoothness(0.075f);
		}

	private:
		Magnum::Timeline const &timeline;
		Magnum::Text::AbstractFont &font;
		Magnum::Text::GlyphCache const &glyphs;
		Magnum::Text::Renderer2D renderer
		{
			font,
			glyphs,
			0.035f,
			Magnum::Text::Alignment::TopRight
		};
		Magnum::Shaders::DistanceFieldVector2D shader;
		std::chrono::system_clock::time_point next = std::chrono::system_clock::now();

		virtual void draw(Magnum::Matrix3 const &transmat, Magnum::SceneGraph::Camera2D &cam) override
		{
			std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
			if(now > next)
			{
				auto const fps = std::to_string(1.0f/timeline.previousFrameDuration());
				renderer.render(fps);
				using namespace std::literals::chrono_literals;
				next = now + 500ms;
			}
			shader
				.setTransformationProjectionMatrix(cam.projectionMatrix() * Magnum::Matrix3::translation(1.0f/cam.projectionMatrix().rotationScaling().diagonal()));
			renderer.mesh().draw(shader);
		}
	};
}}

#endif