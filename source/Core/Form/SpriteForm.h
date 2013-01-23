#ifndef CORE_FORM_SPRITE_H_
#define CORE_FORM_SPRITE_H_
/********************************************
	class:	Mesh2DForm
	usage:	
********************************************/
	/*** common header ***/
#include "Defines.h"
	/*** extra headers if needed ***/
#include "Core/Form/Form2D.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class Interface;
	class SpriteInfo;
	class Spritesheet;
}

namespace Core
{
	class Sprite : public Form2D
	{
	private:
		static const String _type;
		Graphics::Spritesheet* _sheet;
		uint _currentSprite;

	public:
		Sprite();
		~Sprite();

		void DrawSelf(Graphics::Interface& gfx);
		void AnimateSelf();
		const String& getType() const;

		void setSpritesheet(Graphics::Spritesheet& sheet);
		void setSpriteInfo(uint spriteHandle);
	};



	/*
	class Mesh2DForm : public Form
	{
	private:
		static const String _type;

		Image* _image;
		shared_ptr<Texture> _texture;

		Mesh2DForm(Entity& owner, const String& texture, const String& image);
		
		class Maker : public FactoryRouter<Form>
		{
		private:
			static Maker _self;
		public:
			Maker()
				: FactoryRouter(Mesh2DForm::_type)
			{}

			spForm MakeType(ticpp::Element& xml, Entity& owner)
			{
				String texture = xml.FirstChildElement("Texture")->GetText();
				ticpp::Element* img = xml.FirstChildElement("Image", false);
				if(img == NULL)
					return Mesh2DForm::Make(owner, texture);
				return Mesh2DForm::Make(owner, texture, img->GetText());
			}
		};

	public:
		~Mesh2DForm();
		static shared_ptr<Mesh2DForm> Make(Entity& owner, const String& texture, const String& image = "") { return shared_ptr<Mesh2DForm>(new Mesh2DForm(owner, texture, image)); }

		virtual void OnDrawSelf();
		virtual void OnAnimateSelf();

		const String& getType() const;
		Image& getImage();
		shared_ptr<Texture> getTexture();

		void setImage(Image& img);
		void setTexture(shared_ptr<Texture> tex);
	};
	*/
}

#endif //CORE_FORM_SPRITE_H_