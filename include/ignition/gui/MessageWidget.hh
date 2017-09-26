/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef IGNITION_GUI_MESSAGEWIDGET_HH_
#define IGNITION_GUI_MESSAGEWIDGET_HH_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <ignition/math/Color.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/Vector3.hh>

#include "ignition/gui/qt.h"
#include "ignition/gui/CollapsibleWidget.hh"
#include "ignition/gui/PropertyWidget.hh"
#include "ignition/gui/System.hh"

namespace google
{
  namespace protobuf
  {
    class Message;
    class Reflection;
    class FieldDescriptor;
  }
}

namespace ignition
{
  namespace gui
  {
    class MessageWidgetPrivate;

    /// \brief A widget for geometry properties.
    class IGNITION_GUI_VISIBLE GeometryWidget : public PropertyWidget
    {
      Q_OBJECT

      /// \brief A stacked widget containing widgets for configuring
      /// geometry dimensions.
      public: QStackedWidget *geomDimensionWidget;

      /// \brief A spin box for the length of the geometry.
      public: QWidget *geomLengthSpinBox;

      /// \brief A label for the length widget.
      public: QWidget *geomLengthLabel;

      /// \brief A label for the unit of the length widget.
      public: QWidget *geomLengthUnitLabel;

      /// \brief A line edit for the mesh filename.
      public: QWidget *geomFilenameLineEdit;

      /// \brief A label for the mesh filename widget.
      public: QWidget *geomFilenameLabel;

      /// \brief A button for selecting the mesh file.
      public: QWidget *geomFilenameButton;

      /// \brief Callback when the geometry type is changed.
      /// \param[in] _text New geometry type in string.
      private slots: void OnGeometryTypeChanged(const QString &_text);

      /// \brief Callback when geometry size changes.
      /// \param[in] _value Size value that changed.
      private slots: void OnGeometrySizeChanged(const double _value);

      /// \brief Callback when the file button is clicked.
      private slots: void OnSelectFile();

      /// \brief Signal emitted when geometry changes.
      Q_SIGNALS: void GeometryChanged();
    };

    /// \brief A widget for density properties.
    class IGNITION_GUI_VISIBLE DensityWidget : public PropertyWidget
    {
      Q_OBJECT

      /// \brief Constructor
      public: DensityWidget();

      /// \brief Updates the widget's density value.
      /// \param[in] _density New density value.
      public: void SetDensity(const double _density);

      /// \brief Accessor for the widget's density value.
      /// \return The density value.
      public: double Density() const;

      /// \brief Callback when the density combo box is changed.
      /// \param[in] _text New density type in string.
      private slots: void OnComboBoxChanged(const QString &_text);

      /// \brief Callback when the density spin box is changed.
      /// \param[in] _text New density value in string.
      private slots: void OnSpinBoxChanged(const QString &_text);

      /// \brief Signal emitted when density has changed.
      /// \param[in] _value Density value.
      Q_SIGNALS: void DensityValueChanged(const double &_value);

      /// \brief A combo box for density according to material.
      public: QComboBox *comboBox;

      /// \brief A spin box for density value.
      public: QDoubleSpinBox *spinBox;

      /// \brief Current density value.
      private: double density;
    };

    /// \brief A widget for configuring enum values.
    class IGNITION_GUI_VISIBLE EnumWidget : public PropertyWidget
    {
      Q_OBJECT

      /// brief Signal an enum value change event.
      /// \param[in] _value New enum value in string.
      Q_SIGNALS: void EnumValueChanged(const QString &_value);

      /// brief Callback when the enum value is changed.
      /// \param[in] _value New enum value in string.
      private slots: void EnumChanged(const QString &_value);
    };

    /// \brief A widget generated from a google protobuf message.
    class IGNITION_GUI_VISIBLE MessageWidget : public QWidget
    {
      Q_OBJECT

      /// \brief Constructor
      public: MessageWidget();

      /// \brief Destructor
      public: ~MessageWidget();

      /// \brief Load from a google protobuf message.
      /// \param[in] _msg Message to load from.
      public: void Load(const google::protobuf::Message *_msg);

      /// \brief Get the updated message.
      /// \return Updated message.
      public: google::protobuf::Message *Msg();

      /// \brief Signal that a property widget's value has changed.
      /// \param[in] _name Scoped name of widget.
      /// \param[in] _value New value.
      signals: void ValueChanged(const std::string &_name, const QVariant _value);

      /// \brief Set whether a child widget should be visible.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _visible True to set the widget to be visible.
      public: void SetWidgetVisible(const std::string &_name, bool _visible);

      /// \brief Get whether a child widget is visible.
      /// \param[in] _name Name of the child widget.
      /// \return True if the widget is visible.
      public: bool WidgetVisible(const std::string &_name) const;

      /// \brief Set whether a child widget should be read-only.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _visible True to set the widget to be read-only.
      public: void SetWidgetReadOnly(const std::string &_name, bool _readOnly);

      /// \brief Get whether a child widget is read-only.
      /// \param[in] _name Name of the child widget.
      /// \return True if the widget is read-only.
      public: bool WidgetReadOnly(const std::string &_name) const;

      /// \brief Update the widgets from a message.
      /// \param[in] _msg Message used for updating the widgets.
      public: void UpdateFromMsg(const google::protobuf::Message *_msg);

      /// \brief Set a value of a property widget.
      /// \param[in] _name Name of the property widget.
      /// \param[in] _value Value to set to.
      /// \return True if the value is set successfully.
      public: bool SetPropertyValue(const std::string &_name,
                                    const QVariant _value);

      /// \brief Set an integer value to a child widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the value is set successfully.
      public: bool SetIntWidgetValue(const std::string &_name, int _value);

      /// \brief Set an unsigned integer value to a child widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the value is set successfully.
      public: bool SetUIntWidgetValue(const std::string &_name, unsigned int
          _value);

      /// \brief Set a color value to a child widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the value is set successfully.
      public: bool SetColorWidgetValue(const std::string &_name,
          const math::Color &_value);

      /// \brief Set a pose value to a child widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the value is set successfully.
      public: bool SetPoseWidgetValue(const std::string &_name,
          const math::Pose3d &_value);

      /// \brief Set a geometry value to a child widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _value Type of geometry.
      /// \param[in] _dimensions Dimensions of geometry.
      /// \return True if the value is set successfully.
      public: bool SetGeometryWidgetValue(const std::string &_name,
          const std::string &_value,
          const math::Vector3d &_dimensions,
          const std::string &_uri = "");

      /// \brief Set a density value to a child widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _value Density value to set to.
      /// \return True if the value is set successfully.
      public: bool SetDensityWidgetValue(const std::string &_name,
          const double _value);

      /// \brief Set an enum value to a child widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the value is set successfully.
      public: bool SetEnumWidgetValue(const std::string &_name,
          const std::string &_value);

      /// \brief Add an item to a child enum widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _itemText Enum text value.
      /// \return True if the item is added successfully.
      public: bool AddItemEnumWidget(const std::string &_name,
          const std::string &_itemText);

      /// \brief Remove an item from a child enum widget.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _itemText Text of the enum value.
      /// \return True if the item is removed successfully.
      public: bool RemoveItemEnumWidget(const std::string &_name,
          const std::string &_itemText);

      /// \brief Remove all items from a child enum widget.
      /// \param[in] _name Name of the child widget.
      /// \return True if successful.
      public: bool ClearEnumWidget(const std::string &_name);

      /// \brief Get value from a property widget.
      /// \param[in] _name Name of the property widget.
      /// \return Value as QVariant.
      public: QVariant PropertyValue(const std::string &_name) const;

      /// \brief Get an integer value from a child widget.
      /// \param[in] _name Name of the child widget.
      /// \return Integer value.
      public: int IntWidgetValue(const std::string &_name) const;

      /// \brief Get an unsigned integer value from a child widget.
      /// \param[in] _name Name of the child widget.
      /// \return Unsigned integer value.
      public: unsigned int UIntWidgetValue(const std::string &_name) const;

      /// \brief Get a color value from a child widget.
      /// \param[in] _name Name of the child widget.
      /// \return Color value.
      public: math::Color ColorWidgetValue(const std::string &_name) const;

      /// \brief Get a pose value from a child widget.
      /// \param[in] _name Name of the child widget.
      /// \return Pose value.
      public: math::Pose3d PoseWidgetValue(
          const std::string &_name) const;

      /// \brief Get a geometry value from a child widget.
      /// \param[in] _name Name of the child widget.
      /// \param[out] _dimensions Dimensions of geometry.
      /// \param[out] _uri URI of the geometry mesh, if any.
      /// \return Type of geometry.
      public: std::string GeometryWidgetValue(const std::string &_name,
          math::Vector3d &_dimensions, std::string &_uri) const;

      /// \brief Get a density value from a child widget.
      /// \param[in] _name Name of the child widget.
      /// \return Density value.
      public: double DensityWidgetValue(const std::string &_name) const;

      /// \brief Get an enum value from a child widget.
      /// \param[in] _name Name of the child widget.
      /// \return Enum value.
      public: std::string EnumWidgetValue(const std::string &_name) const;

      /// \brief Create a widget which has a button header which collapses
      /// the field widget.
      /// \param[in] _name Header name.
      /// \param[out] _childWidget Widget which will be collapsed.
      /// \param[in] _level Level of the widget in the tree.
      /// \return The group widget.
      public: CollapsibleWidget *CreateCollapsibleWidget(const std::string &_name,
          PropertyWidget *_childWidget, const int _level = 0);

      /// \brief Create a widget for configuring an unsigned integer value.
      /// \param[in] _key A key that is used as a label for the widget.
      /// \param[in] _level Level of the widget in the tree.
      /// \return The newly created widget.
      public: PropertyWidget *CreateUIntWidget(const std::string &_key,
          const int _level = 0);

      /// \brief Create a widget for configuring an integer value.
      /// \param[in] _key A key that is used as a label for the widget.
      /// \param[in] _level Level of the widget in the tree.
      /// \return The newly created widget.
      public: PropertyWidget *CreateIntWidget(const std::string &_key,
          const int _level = 0);

      /// \brief Create a widget for configuring a color value.
      /// \param[in] _key A key that is used as a label for the widget.
      /// \param[in] _level Level of the widget in the tree.
      /// \return The newly created widget.
      public: PropertyWidget *CreateColorWidget(const std::string &_key,
          const int _level = 0);

      /// \brief Create a widget for configuring a pose value.
      /// \param[in] _key A key that is used as a label for the widget.
      /// \param[in] _level Level of the widget in the tree.
      /// \return The newly created widget.
      public: PropertyWidget *CreatePoseWidget(const std::string &_key,
          const int _level = 0);

      /// \brief Create a widget for configuring a geometry value.
      /// \param[in] _key A key that is used as a label for the widget.
      /// \param[in] _level Level of the widget in the tree.
      /// \return The newly created widget.
      public: PropertyWidget *CreateGeometryWidget(const std::string &_key,
          const int _level = 0);

      /// \brief Create a widget for configuring an enum value.
      /// \param[in] _key A key that is used as a label for the widget.
      /// \param[in] _values A list of enum values in string.
      /// \param[in] _level Level of the widget in the tree.
      /// \return The newly created widget.
      public: PropertyWidget *CreateEnumWidget(const std::string &_key,
          const std::vector<std::string> &_values, const int _level = 0);

      /// \brief Create a widget for setting a density value.
      /// \param[in] _key A key that is used as a label for the widget.
      /// \param[in] _level Level of the widget in the tree.
      /// \return The newly created widget.
      public: PropertyWidget *CreateDensityWidget(const std::string &_key,
          const int _level = 0);

      /// \brief Register a child widget as a child of this widget, so it can
      /// be updated. Note that the widget is not automatically added to a
      /// layout.
      /// \param[in] _name Unique name to indentify the child within this widget
      /// \param[in] _child Child widget to be added. It doesn't need to be a
      /// PropertyWidget.
      /// \return True if child successfully added.
      public: bool AddPropertyWidget(const std::string &_name,
          PropertyWidget *_child);

      /// \brief Insert a layout into the config widget's layout at a specific
      /// position.
      /// \param[in] _layout The layout to be inserted.
      /// \param[in] _pos The position to insert at, 0 being the top.
      public: void InsertLayout(QLayout *_layout, int _pos);

      /// \brief Get a config child widget by its name.
      /// \param[in] _name Scoped name of the child widget.
      /// \return The child widget with the given name or nullptr if it wasn't
      /// found.
      public: PropertyWidget *PropertyWidgetByName(
          const std::string &_name) const;

      /// \brief Get the number of child widgets.
      /// \return The number of child widgets.
      public: unsigned int PropertyWidgetCount() const;

      /// \brief Get a style sheet in string format, to be applied to a child
      /// config widget with setStyleSheet.
      /// \param[in] _type Type of style sheet, such as "warning", "active",
      /// "normal".
      /// \param[in] _level Level of widget in the tree.
      /// \return Style sheet as string. Returns an empty string if _type is
      /// unknown.
      public: static QString StyleSheet(const std::string &_type,
          const int _level = 0);

      /// \brief List of colors used for the background of widgets according to
      /// their level.
      public: static const std::vector<QString> bgColors;

      /// \brief List of colors used for widget areas according to their level.
      public: static const std::vector<QString> widgetColors;

      /// \brief Red color used for "red" or "x" fields.
      public: static const QString redColor;

      /// \brief Green color used for "green" or "y" fields.
      public: static const QString greenColor;

      /// \brief Blue color used for "blue" or "z" fields.
      public: static const QString blueColor;

      /// \brief Parse the input message and either create widgets for
      /// configuring fields of the message, or update the widgets with values
      /// from the message.
      /// \param[in] _msg Message.
      /// \param[in] _update True to parse only fields that are specified in
      /// the message rather than all the available fields in the message
      /// \param[in] _name Name used when creating new widgets.
      /// \param[in] _level Level of the widget in the tree.
      /// return Updated widget.
      private: QWidget *Parse(google::protobuf::Message *_msg,
          bool _update = false, const std::string &_name = "",
          const int _level = 0);

      /// \brief Parse a vector3 message.
      /// param[in] _msg Input vector3d message.
      /// return Parsed vector.
      private: math::Vector3d ParseVector3d(
          const google::protobuf::Message *_msg) const;

      /// \brief Update the message field using values from the widgets.
      /// \param[in] _msg Message to be updated.
      /// \param[in] _name Name of parent widget.
      private: void UpdateMsg(google::protobuf::Message *_msg,
          const std::string &_name = "");

      /// \brief Update a vector3d message.
      /// \param[in] _msg Vector3d message to be updated.
      /// \param[in] _value ignition math Vector3d used for updating the
      /// message.
      private: void UpdateVector3dMsg(google::protobuf::Message *_msg,
          const math::Vector3d &_value);

      /// \brief Update a child widget with an unsigned integer value.
      /// \param[in] _widget Pointer to the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the update completed successfully.
      private: bool UpdateUIntWidget(PropertyWidget *_widget,
          const unsigned int _value);

      /// \brief Update a child widget with an integer value.
      /// \param[in] _widget Pointer to the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the update completed successfully.
      private: bool UpdateIntWidget(PropertyWidget *_widget,
           const int _value);

      /// \brief Update a child widget with a color value.
      /// \param[in] _widget Pointer to the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the update completed successfully.
      private: bool UpdateColorWidget(PropertyWidget *_widget,
          const math::Color &_value);

      /// \brief Update a child widget with a pose value.
      /// \param[in] _widget Pointer to the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the update completed successfully.
      private: bool UpdatePoseWidget(PropertyWidget *_widget,
          const math::Pose3d &_value);

      /// \brief Update a child widget with a geometry type and dimensions.
      /// \param[in] _widget Pointer to the child widget.
      /// \param[in] _value Type of geometry.
      /// \param[in] _dimensions Dimensions of the geometry.
      /// \param[in] _uri URI of the geometry mesh, if any.
      /// \return True if the update completed successfully.
      private: bool UpdateGeometryWidget(PropertyWidget *_widget,
          const std::string &_value,
          const math::Vector3d &_dimensions,
          const std::string &_uri = "");

      /// \brief Update a child widget with an enum value.
      /// \param[in] _widget Pointer to the child widget.
      /// \param[in] _value Value to set to.
      /// \return True if the update completed successfully.
      private: bool UpdateEnumWidget(PropertyWidget *_widget,
          const std::string &_value);

      /// \brief Update a child widget with a density value.
      /// \param[in] _widget Pointer to the child widget.
      /// \param[in] _value Density value.
      /// \return True if the update completed successfully.
      private: bool UpdateDensityWidget(PropertyWidget *_widget,
          const double _value);

      /// \brief Get an integer value from a child widget.
      /// \param[in] _widget Pointer to the child widget.
      /// \return Value of the widget.
      private: int IntWidgetValue(PropertyWidget *_widget) const;

      /// \brief Get an unsigned integer value from a child widget.
      /// \param[in] _widget Pointer to the child widget.
      /// \return Value of the widget.
      private: unsigned int UIntWidgetValue(PropertyWidget *_widget) const;

      /// \brief Get a color value from a child widget.
      /// \param[in] _widget Pointer to the child widget.
      /// \return Value of the widget.
      private: math::Color ColorWidgetValue(PropertyWidget *_widget) const;

      /// \brief Get a pose value from a child widget.
      /// \param[in] _widget Pointer to the child widget.
      /// \return Value of the widget.
      private: math::Pose3d PoseWidgetValue(
          PropertyWidget *_widget) const;

      /// \brief Get a geometry value from a child widget.
      /// \param[in] _widget Pointer to the child widget.
      /// \param[out] _dimensions Dimensions of geometry.
      /// \param[out] _uri URI of the geometry mesh, if any.
      /// \return Type of geometry.
      private: std::string GeometryWidgetValue(PropertyWidget *_widget,
          math::Vector3d &_dimensions, std::string &_uri) const;

      /// \brief Get an enum value from a child widget.
      /// \param[in] _widget Pointer to the child widget.
      /// \return Value of the widget.
      private: std::string EnumWidgetValue(PropertyWidget *_widget) const;

      /// \brief Received item selection user input.
      /// \param[in] _item Item selected.
      /// \param[in] _column Column index.
      private slots: void OnItemSelection(QTreeWidgetItem *_item,
          const int _column);

      /// \brief Callback when a uint widget's value has changed.
      private slots: void OnUIntValueChanged();

      /// \brief Callback when an int widget's value has changed.
      private slots: void OnIntValueChanged();

      /// \brief Callback when a color widget's value has changed.
      private slots: void OnColorValueChanged();

      /// \brief Callback when a color widget's value has changed from
      /// the color picker.
      /// \param[in] _value New color value
      private slots: void OnColorValueChanged(const QColor _value);

      /// \brief Callback when a pose widget's value has changed.
      private slots: void OnPoseValueChanged();

      /// \brief Callback when a geometry widget's value has changed.
      private slots: void OnGeometryValueChanged();

      /// \brief Callback when a geometry widget's value has changed.
      /// \param[in] _value Value which the QComboBox changed to.
      private slots: void OnGeometryValueChanged(const int _value);

      /// \brief Callback when an enum widget's enum value has changed.
      /// \param[in] _value New enum value in string.
      private slots: void OnEnumValueChanged(const QString &_value);

      /// \brief Callback when an enum widget's enum value has changed.
      /// \param[in] _value New enum value in string.
      private slots: void OnCustomColorDialog();

      /// \brief Signal that a color widget's value has changed.
      /// \param[in] _name Scoped name of widget.
      /// \param[in] _value New color.
      Q_SIGNALS: void ColorValueChanged(const QString &_name,
          const math::Color &_value);

      /// \brief Signal that a pose widget's value has changed.
      /// \param[in] _name Scoped name of widget.
      /// \param[in] _pose New pose.
      Q_SIGNALS: void PoseValueChanged(const QString &_name,
          const math::Pose3d &_pose);

      /// \brief Signal that a geometry widget's value has changed.
      /// \param[in] _name Scoped name of widget.
      /// \param[in] _value New geometry name, such as "box".
      /// \param[in] _dimensions New dimensions.
      /// \param[in] _uri New uri, for meshes.
      Q_SIGNALS: void GeometryValueChanged(const std::string &_name,
          const std::string &_value,
          const math::Vector3d &_dimensions,
          const std::string &_uri);

      /// \brief Signal that an enum widget's enum value has changed.
      /// \param[in] _name Scoped name of widget.
      /// \param[in] _value New enum value string.
      Q_SIGNALS: void EnumValueChanged(const QString &_name,
          const QString &_value);

      /// \brief Signal emitted when density value changes.
      /// \param[in] _value Density value.
      Q_SIGNALS: void DensityValueChanged(const double &_value);

      /// \brief Signal emitted when mass value changes.
      /// \param[in] _value Mass value.
      Q_SIGNALS: void MassValueChanged(const double &_value);

      /// \brief Callback when density value changes in child widget.
      /// \param[in] _value Density value.
      private slots: void OnDensityValueChanged(const double _value);

      /// \brief Callback when mass value changes in child widget.
      /// \param[in] _value Mass value.
      private slots: void OnMassValueChanged(const double _value);

      /// \brief Callback when geometry changes.
      private slots: void OnGeometryChanged();

      /// \brief Signal emitted when geometry changes.
      Q_SIGNALS: void GeometryChanged();

      /// \brief Qt event filter currently used to filter mouse wheel events.
      /// \param[in] _obj Object that is watched by the event filter.
      /// \param[in] _event Qt event.
      /// \return True if the event is handled.
      private: bool eventFilter(QObject *_obj, QEvent *_event);

      /// \internal
      /// \brief Pointer to private data.
      private: std::unique_ptr<MessageWidgetPrivate> dataPtr;
    };
  }
}
#endif
