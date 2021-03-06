/*
 *    Copyright 2012, 2013 Thomas Schöps
 *    Copyright 2019, 2020 Kai Pastor
 *
 *    This file is part of OpenOrienteering.
 *
 *    OpenOrienteering is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    OpenOrienteering is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with OpenOrienteering.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "map_dialog_stretch.h"

#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include "core/georeferencing.h"
#include "core/map.h"
#include "templates/template.h"
#include "gui/util_gui.h"


namespace OpenOrienteering {

StretchMapDialog::StretchMapDialog(QWidget* parent, Map* map, double stretch_factor) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint), stretch_factor(stretch_factor), map(map)
{
	setWindowTitle(tr("Change scale factor"));
	
	QFormLayout* layout = new QFormLayout();
	
	layout->addRow(Util::Headline::create(tr("Scaling parameters")));
	
	layout->addRow(new QLabel(tr("Scaling center:")));
	
	//: Scaling center point
	center_origin_radio = new QRadioButton(tr("Map coordinate system origin"));
	if (!map->getGeoreferencing().isValid())
		center_origin_radio->setChecked(true);
	layout->addRow(center_origin_radio);
	
	//: Scaling center point
	center_georef_radio = new QRadioButton(tr("Georeferencing reference point"));
	if (map->getGeoreferencing().isValid())
		center_georef_radio->setChecked(true);
	else
		center_georef_radio->setEnabled(false);
	layout->addRow(center_georef_radio);
	
	//: Scaling center point
	center_other_radio = new QRadioButton(tr("Other point,"));
	layout->addRow(center_other_radio);
	
	other_x_edit = Util::SpinBox::create<MapCoordF>();
	//: x coordinate
	layout->addRow(tr("X:"), other_x_edit);
	
	other_y_edit = Util::SpinBox::create<MapCoordF>();
	//: y coordinate
	layout->addRow(tr("Y:"), other_y_edit);
	
	layout->addItem(Util::SpacerItem::create(this));
	layout->addRow(Util::Headline::create(tr("Options")));
	
	adjust_georeferencing_check = new QCheckBox(tr("Adjust georeferencing reference point"));
	if (map->getGeoreferencing().isValid())
		adjust_georeferencing_check->setChecked(true);
	else
		adjust_georeferencing_check->setEnabled(false);
	layout->addRow(adjust_georeferencing_check);
	
	adjust_templates_check = new QCheckBox(tr("Scale non-georeferenced templates"));
	bool have_non_georeferenced_template = false;
	for (int i = 0; i < map->getNumTemplates() && !have_non_georeferenced_template; ++i)
		have_non_georeferenced_template = !map->getTemplate(i)->isTemplateGeoreferenced();
	for (int i = 0; i < map->getNumClosedTemplates() && !have_non_georeferenced_template; ++i)
		have_non_georeferenced_template = !map->getClosedTemplate(i)->isTemplateGeoreferenced();
	if (have_non_georeferenced_template)
		adjust_templates_check->setChecked(true);
	else
		adjust_templates_check->setEnabled(false);
	layout->addRow(adjust_templates_check);
	
	
	QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
	ok_button = button_box->button(QDialogButtonBox::Ok);
	
	auto* box_layout = new QVBoxLayout();
	box_layout->addLayout(layout);
	box_layout->addItem(Util::SpacerItem::create(this));
	box_layout->addStretch();
	box_layout->addWidget(button_box);
	
	setLayout(box_layout);
	
	connect(center_origin_radio, &QAbstractButton::clicked, this, &StretchMapDialog::updateWidgets);
	connect(center_georef_radio, &QAbstractButton::clicked, this, &StretchMapDialog::updateWidgets);
	connect(center_other_radio, &QAbstractButton::clicked, this, &StretchMapDialog::updateWidgets);
	connect(button_box, &QDialogButtonBox::accepted, this, &StretchMapDialog::okClicked);
	connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
	
	updateWidgets();
}

void StretchMapDialog::updateWidgets()
{
	other_x_edit->setEnabled(center_other_radio->isChecked());
	other_y_edit->setEnabled(center_other_radio->isChecked());
	adjust_georeferencing_check->setEnabled(!center_georef_radio->isChecked());
}

void StretchMapDialog::okClicked()
{
	MapCoord center = MapCoord(0, 0);
	if (center_georef_radio->isChecked())
		center = map->getGeoreferencing().getMapRefPoint();
	else if (center_other_radio->isChecked())
		center = MapCoord(other_x_edit->value(), -1 * other_y_edit->value());
	
	map->changeScale(map->getScaleDenominator(), stretch_factor, center, false, true, adjust_georeferencing_check->isChecked(), adjust_templates_check->isChecked());
	accept();
}


}  // namespace OpenOrienteering
