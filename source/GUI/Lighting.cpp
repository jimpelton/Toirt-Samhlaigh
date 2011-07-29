/*
 * Lighting.cpp - Methods for Lighting class.
 *
 * Author: Patrick O'Leary
 * Created: September 25, 2008
 * Copyright: 2008. All rights reserved.
 */
#include <iostream>

/* Vrui includes to use the Vrui interface */
#include <Vrui/Vrui.h>

/* Vrui includes */
#include <GL/GLColor.h>
#include <GLMotif/WidgetManager.h>

#include <COLORMAP/RGBAColor.h>
#include <GRAPHIC/Scene.h>
#include <GUI/Lighting.h>
#include <GUI/LightWidget.h>
#include <GUI/SwatchesWidget.h>
#include <Toirt_Samhlaigh.h>

/*
 * Lighting - Constructor for Lighting class.
 * 		extends GLMotif::PopupWindow
 */
Lighting::Lighting(Scene * _scene, Toirt_Samhlaigh & _toirt_samhlaigh, float * _extent, float * _origin) :
    GLMotif::PopupWindow("LightingPopup", Vrui::getWidgetManager(), "Lighting"), ambient(true), diffuse(false), scene(_scene),
            specular(false), toirt_samhlaigh(_toirt_samhlaigh) {
    extent = new float[3];
    extent[0] = _extent[0];
    extent[1] = _extent[1];
    extent[2] = _extent[2];
    origin = new float[3];
    origin[0] = _origin[0];
    origin[1] = _origin[1];
    origin[2] = _origin[2];
    initialize();
}

/*
 * ~Lighting - Destructor for Lighting class.
 */
Lighting::~Lighting(void) {
}

/*
 * ambientCallback
 *
 * parameter callBackData - GLMotif::ToggleButton::ValueChangedCallbackData*
 */
void Lighting::ambientCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData) {
    if (strcmp(callBackData->toggle->getName(), "AmbientButton") == 0) {
        ambientButton->setToggle(true);
        diffuseButton->setToggle(false);
        specularButton->setToggle(false);
        ambient = true;
        diffuse = false;
        specular = false;
        float * _color = lightWidget->getAmbientColor();
        for (int i = 0; i < 3; ++i) {
            colorSliders[i]->setValue(_color[i]);
        }
        colorPane->setBackgroundColor(_color);
        ambientPane->setBackgroundColor(_color);
    }
    Vrui::requestUpdate();
} // end ambientCallback()

/*
 * colorSliderCallback - Callback of change to color slider value.
 *
 * parameter callbackData - Misc::CallbackData*
 */
void Lighting::colorSliderCallback(Misc::CallbackData* callbackData) {
    RGBAColor* rgbaColor = new RGBAColor(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 3; ++i) {
        rgbaColor->setValues(i, float(colorSliders[i]->getValue()));
    }
    rgbaColor->setValues(3, 0.1f);
    float * _color = rgbaColor->getValues();
    colorPane->setBackgroundColor(_color);
    if (ambient) {
        ambientPane->setBackgroundColor(_color);
        lightWidget->setAmbientColor(_color);
        scene->setAmbientColor(_color[0],_color[1],_color[2],_color[3]);
    } else if (diffuse) {
        diffusePane->setBackgroundColor(_color);
         lightWidget->setDiffuseColor(_color);
         scene->setDiffuseColor(_color[0],_color[1],_color[2],_color[3]);
    } else {
        specularPane->setBackgroundColor(_color);
         lightWidget->setSpecularColor(_color);
         scene->setSpecularColor(_color[0],_color[1],_color[2],_color[3]);
    }
} // end colorSliderCallback()

/*
 * colorSwatchesWidgetCallback
 *
 * parameter callbackData - Misc::CallbackData*
 */
void Lighting::colorSwatchesWidgetCallback(Misc::CallbackData* callbackData) {
    float * _color = swatchesWidget->getCurrentColor();
    RGBAColor* rgbaColor = new RGBAColor(_color[0], _color[1], _color[2], 0.1f);
    for (int i = 0; i < 3; ++i) {
        colorSliders[i]->setValue(_color[i]);
    }
    colorPane->setBackgroundColor(rgbaColor->getValues());
    if (ambient) {
        ambientPane->setBackgroundColor(_color);
        lightWidget->setAmbientColor(_color);
        scene->setAmbientColor(_color[0],_color[1],_color[2],_color[3]);
    } else if (diffuse) {
        diffusePane->setBackgroundColor(_color);
         lightWidget->setDiffuseColor(_color);
         scene->setDiffuseColor(_color[0],_color[1],_color[2],_color[3]);
    } else {
        specularPane->setBackgroundColor(_color);
         lightWidget->setSpecularColor(_color);
         scene->setSpecularColor(_color[0],_color[1],_color[2],_color[3]);
    }
} // end colorSwatchesWidgetCallback()

/*
 * createColorEditor - Create color editor.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter transferFunction2_5DDialog - GLMotif::RowColumn*&
 * return - GLMotif::RowColumn*
 */
GLMotif::RowColumn* Lighting::createColorEditor(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& lightingDialog) {
    GLMotif::RowColumn* colorEditor = new GLMotif::RowColumn("ColorEditor", lightingDialog, false);
    colorEditor->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    createColorSwatchesWidget(styleSheet, colorEditor);
    GLMotif::RowColumn* colorSlidersBox = createColorSliderBox(styleSheet, colorEditor);
    colorSlidersBox->manageChild();
    colorPane = new GLMotif::Blind("ColorPane", colorEditor);
    colorPane->setBorderWidth(styleSheet.size * 0.5f);
    colorPane->setBorderType(GLMotif::Widget::LOWERED);
    colorPane->setBackgroundColor(GLMotif::Color(0.5f, 0.5f, 0.5f));
    colorPane->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 5.0f, styleSheet.fontHeight * 5.0f, 0.0f));
    return colorEditor;
} // end createColorEditor()

/*
 * createColorSwatchesWidget
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorEditor - GLMotif::RowColumn*&
 */
void Lighting::createColorSwatchesWidget(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorEditor) {
    swatchesWidget = new SwatchesWidget("SwatchesWidget", colorEditor);
    swatchesWidget->setBorderWidth(styleSheet.size * 0.5f);
    swatchesWidget->setBorderType(GLMotif::Widget::LOWERED);
    swatchesWidget->setForegroundColor(GLMotif::Color(0.0f, 1.0f, 0.0f));
    swatchesWidget->setMarginWidth(styleSheet.size);
    swatchesWidget->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 15.0, styleSheet.fontHeight * 5.0, 0.0f));
    swatchesWidget->getColorChangedCallbacks().add(this, &Lighting::colorSwatchesWidgetCallback);
} // end createColorSwatchesWidget()

/*
 * createColorSlider - Create color slider.
 *
 * parameter title - const char*
 * parameter color - GLMotif::Color
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorSliderBox - GLMotif::RowColumn*
 * return - GLMotif::Slider*
 */
GLMotif::Slider* Lighting::createColorSlider(const char* title, GLMotif::Color color, const GLMotif::StyleSheet& styleSheet,
        GLMotif::RowColumn* colorSlidersBox) {
    GLMotif::Slider* colorSlider = new GLMotif::Slider(title, colorSlidersBox, GLMotif::Slider::VERTICAL, styleSheet.fontHeight
            * 5.0f);
    colorSlider->setSliderColor(color);
    colorSlider->setValueRange(0.0f, 1.0f, 0.01f);
    colorSlider->setValue(0.5f);
    colorSlider->getValueChangedCallbacks().add(this, &Lighting::colorSliderCallback);
    return colorSlider;
} // end createColorSlider()

/*
 * createColorSliderBox - Create box to contain color slider bars.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorEditor - GLMotif::RowColumn*
 * return - GLMotif::RowColumn*
 */
GLMotif::RowColumn* Lighting::createColorSliderBox(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn* colorEditor) {
    GLMotif::RowColumn* colorSlidersBox = new GLMotif::RowColumn("ColorSliders", colorEditor, false);
    colorSlidersBox->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    colorSlidersBox->setPacking(GLMotif::RowColumn::PACK_GRID);
    createColorSliders(styleSheet, colorSlidersBox);
    return colorSlidersBox;
} // end createColorSliderBox()

/*
 * createColorSliders - Create RGB color sliders.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorSliderBox - GLMotif::RowColumn*
 */
void Lighting::createColorSliders(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn* colorSlidersBox) {
    colorSliders[0] = createColorSlider("RedSlider", GLMotif::Color(1.0f, 0.0f, 0.0f), styleSheet, colorSlidersBox);
    colorSliders[1] = createColorSlider("GreenSlider", GLMotif::Color(0.0f, 1.0f, 0.0f), styleSheet, colorSlidersBox);
    colorSliders[2] = createColorSlider("BlueSlider", GLMotif::Color(0.0f, 0.0f, 1.0f), styleSheet, colorSlidersBox);
} // end createColorSliders()

/*
 * createLightProperties
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter lightingDialog - GLMotif::RowColumn*
 */
void Lighting::createLightProperties(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn * lightingDialog) {
    GLMotif::RowColumn * lightPropertiesRowColumn = new GLMotif::RowColumn("LightPropertiesRowColumn", lightingDialog, false);
    lightPropertiesRowColumn->setOrientation(GLMotif::RowColumn::VERTICAL);
    lightPropertiesRowColumn->setPacking(GLMotif::RowColumn::PACK_GRID);
    GLMotif::RowColumn * colorRowColumn = new GLMotif::RowColumn("ColorRowColumn", lightPropertiesRowColumn, false);
    colorRowColumn->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    colorRowColumn->setPacking(GLMotif::RowColumn::PACK_GRID);
    ambientButton = new GLMotif::ToggleButton("AmbientButton", colorRowColumn, "Ambient");
    ambientButton->setToggle(true);
    ambientButton->getSelectCallbacks().add(this, &Lighting::ambientCallback);
    ambientPane = new GLMotif::Blind("AmbientPane", colorRowColumn);
    ambientPane->setBorderWidth(styleSheet.size * 0.5f);
    ambientPane->setBorderType(GLMotif::Widget::LOWERED);
    ambientPane->setBackgroundColor(GLMotif::Color(0.5f, 0.5f, 0.5f));
    ambientPane->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 5.0f, styleSheet.fontHeight * 1.0f, 0.0f));
    diffuseButton = new GLMotif::ToggleButton("DiffuseButton", colorRowColumn, "Diffuse");
    diffuseButton->setToggle(false);
    diffuseButton->getSelectCallbacks().add(this, &Lighting::diffuseCallback);
    diffusePane = new GLMotif::Blind("DiffusePane", colorRowColumn);
    diffusePane->setBorderWidth(styleSheet.size * 0.5f);
    diffusePane->setBorderType(GLMotif::Widget::LOWERED);
    diffusePane->setBackgroundColor(GLMotif::Color(0.5f, 0.5f, 0.5f));
    diffusePane->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 5.0f, styleSheet.fontHeight * 1.0f, 0.0f));
    specularButton = new GLMotif::ToggleButton("SpecularButton", colorRowColumn, "Specular");
    specularButton->setToggle(false);
    specularButton->getSelectCallbacks().add(this, &Lighting::specularCallback);
    specularPane = new GLMotif::Blind("SpecularPane", colorRowColumn);
    specularPane->setBorderWidth(styleSheet.size * 0.5f);
    specularPane->setBorderType(GLMotif::Widget::LOWERED);
    specularPane->setBackgroundColor(GLMotif::Color(0.5f, 0.5f, 0.5f));
    specularPane->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 5.0f, styleSheet.fontHeight * 1.0f, 0.0f));
    colorRowColumn->manageChild();
    lightPropertiesRowColumn->manageChild();

} // end createLightProperties()

/*
 * createLightWidgetRowColumn
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter lightingDialog - GLMotif::RowColumn*
 */
void Lighting::createLightWidgetRowColumn(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn * lightingDialog) {
    GLMotif::RowColumn * lightWidgetRowColumn = new GLMotif::RowColumn("LightWidgetRowColumn", lightingDialog, false);
    lightWidgetRowColumn->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    lightWidgetRowColumn->setPacking(GLMotif::RowColumn::PACK_TIGHT);
    createLightWidget(styleSheet, lightWidgetRowColumn);
    lightWidgetRowColumn->manageChild();
} // end createLightWidgetRowColumn()

/*
 * createLightWidget
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter sliceAndHistogramRowColumn - GLMotif::RowColumn*
 */
void Lighting::createLightWidget(const GLMotif::StyleSheet & styleSheet, GLMotif::RowColumn * & lightWidgetRowColumn) {
    lightWidget = new LightWidget(scene, "LightWidget", lightWidgetRowColumn);
    lightWidget->setBorderWidth(styleSheet.size * 0.5f);
    lightWidget->setBorderType(GLMotif::Widget::LOWERED);
    lightWidget->setForegroundColor(GLMotif::Color(0.0f, 1.0f, 0.0f));
    lightWidget->setMarginWidth(styleSheet.size);
    lightWidget->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 20.0, styleSheet.fontHeight * 20.0, 0.0f));
    lightWidget->setObjectSize(styleSheet.fontHeight * 7.5f);
    lightWidget->setExtent(extent);
    lightWidget->setOrigin(extent);
} // end createLightWidget()

/*
 * diffuseCallback
 *
 * parameter callBackData - GLMotif::ToggleButton::ValueChangedCallbackData*
 */
void Lighting::diffuseCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData) {
    if (strcmp(callBackData->toggle->getName(), "DiffuseButton") == 0) {
        ambientButton->setToggle(false);
        diffuseButton->setToggle(true);
        specularButton->setToggle(false);
        ambient = false;
        diffuse = true;
        specular = false;
        float * _color = lightWidget->getDiffuseColor();
        for (int i = 0; i < 3; ++i) {
            colorSliders[i]->setValue(_color[i]);
        }
        colorPane->setBackgroundColor(_color);
        diffusePane->setBackgroundColor(_color);
    }
    Vrui::requestUpdate();
} // end diffuseCallback()

/*
 * initialize - Initialize the GUI for the Lighting class.
 */
void Lighting::initialize(void) {
    ambientColor = new RGBAColor(0.0f, 0.0f, 0.0f, 0.0f);
    diffuseColor = new RGBAColor(0.0f, 0.0f, 0.0f, 0.0f);
    specularColor = new RGBAColor(0.0f, 0.0f, 0.0f, 0.0f);
    const GLMotif::StyleSheet& styleSheet = *Vrui::getWidgetManager()->getStyleSheet();
    GLMotif::RowColumn* lightingDialog = new GLMotif::RowColumn("LightingDialog", this, false);
    createLightWidgetRowColumn(styleSheet, lightingDialog);
    createLightProperties(styleSheet, lightingDialog);
    GLMotif::RowColumn* colorEditor = createColorEditor(styleSheet, lightingDialog);
    colorEditor->manageChild();
    lightingDialog->manageChild();
}

/*
 * specularCallback
 *
 * parameter callBackData - GLMotif::ToggleButton::ValueChangedCallbackData*
 */
void Lighting::specularCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData) {
    if (strcmp(callBackData->toggle->getName(), "SpecularButton") == 0) {
        ambientButton->setToggle(false);
        diffuseButton->setToggle(false);
        specularButton->setToggle(true);
        ambient = false;
        diffuse = false;
        specular = true;
        float * _color = lightWidget->getSpecularColor();
        for (int i = 0; i < 3; ++i) {
            colorSliders[i]->setValue(_color[i]);
        }
        colorPane->setBackgroundColor(_color);
        specularPane->setBackgroundColor(_color);
    }
    Vrui::requestUpdate();
} // end specularCallback()
