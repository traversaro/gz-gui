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

#ifndef IGNITION_GUI_STRINGWIDGET_HH_
#define IGNITION_GUI_STRINGWIDGET_HH_

#include <memory>
#include <string>

#include "ignition/gui/qt.h"
#include "ignition/gui/PropertyWidget.hh"
#include "ignition/gui/System.hh"

namespace ignition
{
  namespace gui
  {
    class StringWidgetPrivate;

    /// \brief A widget which holds a string property (std::string).
    class IGNITION_GUI_VISIBLE StringWidget : public PropertyWidget
    {
      Q_OBJECT

      /// \brief String types
      public: enum StringType
      {
        /// \brief Use line for short strings which usually fit in a single
        /// line.
        LINE,

        /// \brief Use text for longer strings which span multiple lines.
        TEXT
      };

      /// \brief Constructor
      /// \param[in] _key Property key value, such as "name", which will be
      /// displayed next to the field which holds the string content.
      /// \param[in] _type The value type, which will determine characteristics
      /// of the line field, such as how large the widget is. Defaults to LINE.
      public: StringWidget(const std::string &_key,
                           const StringType _type = LINE);

      /// \brief Destructor
      public: ~StringWidget();

      /// \brief Inherited from PropertyWidget.
      /// Value will be handled if the variant contains an std::string.
      /// \param[in] _value New value.
      /// \return True if succesfull.
      public: bool SetValue(const QVariant _value);

      /// \brief Inherited from PropertyWidget.
      /// Returns a variant containing the widget's current std::string value.
      /// \return Widget's current value.
      public: QVariant Value() const;

      /// \internal
      /// \brief Pointer to private data.
      private: std::unique_ptr<StringWidgetPrivate> dataPtr;
    };
  }
}
#endif
