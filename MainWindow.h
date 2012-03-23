// Copyright (c) 2011 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//                 Kumar Snehasish <kumar.snehasish@gmail.com>
//
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "typedefs.h"
#include "ui_MainWindow.h"
#include "ui_CreateMesh.h"
#include "ui_CreateMenger.h"

#include <CGAL/Qt/DemosMainWindow.h>
#include <CGAL/Random.h>

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QFileDialog>

#include <QDockWidget>
#include <QTableWidget>
#include <QCheckBox>

class QWidget;

class DialogMesh : public QDialog, public Ui::createMesh
{
  Q_OBJECT

public:
  DialogMesh(QWidget* parent)
  { 
    setupUi (this); 
  }

  int getX() { return xvalue->value(); }
  int getY() { return yvalue->value(); }
  int getZ() { return zvalue->value(); }
};

class DialogMenger : public QDialog, public Ui::createMenger
{
    Q_OBJECT

    public:

    DialogMenger(QWidget *parent)
    {
        setupUi(this);
    }
};

class MainWindow : public CGAL::Qt::DemosMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = 0);

public slots:
  void import_off();
  void add_off();
  void load_off(const QString& fileName, bool clear=true);

  void import_3DTDS();
  void load_3DTDS(const QString& fileName, bool clear=true);
  
  void clear(bool msg=true);

  Dart_handle create_cube();
  void create_3cubes();
  void create_2volumes();
  void create_mesh();
  void create_menger();

  void subdivide();
  void dual_3();
  void voronoi_3();
  void close_volume();
  void remove_filled_volumes();
  void remove_selected_volume();
  void sew3_same_facets();
  void unsew3_all();
  void triangulate_all_facets();

  void onSceneChanged();

  void connectVolumeListHandlers();
  void onCellChanged(int, int);
  void onItemSelectionChanged();
  void onHeaderClicked(int);

  void onMengerInc();
  void onMengerDec();
  void onMengerChange(int);

  void extendVolumesSatisfying(char amask, char negatemask);
  void extendFilledVolumes();
  void extendHiddenVolumes();
  
signals:
  void sceneChanged();
  
protected:
  void onNewVolume(Dart_handle adart);
  void onDeleteVolume(Dart_handle adart);
  void initAllNewVolumes();
  
  Dart_handle make_iso_cuboid(const Point_3 basepoint, LCC::FT lg);

  void connectActions();

  void check_volume_list();
  bool is_volume_in_list(Dart_handle it);
  void update_volume_list();
  void update_volume_list_add(Dart_handle);
  void update_volume_list_remove(unsigned int);
  void update_volume_list_remove(Dart_handle);

  void split_edge_in_three     (Dart_handle dh);
  void split_face_in_three     (Dart_handle dh);
  void split_face_in_nine      (Dart_handle dh);
  void split_vol_in_three      (Dart_handle dh, bool removecenter);
  void split_vol_in_nine       (Dart_handle dh, bool removecenter);
  void split_vol_in_twentyseven(Dart_handle dh);

  Scene scene;
  Timer timer;

  unsigned int nbcube;
  QLabel*      statusMessage;
  DialogMesh   dialogmesh;
  DialogMenger dialogmenger;
  CGAL::Random random;

  unsigned int mengerLevel;
  unsigned int mengerFirstVol;

  QDockWidget* volumeListDock;
  QTableWidget* volumeList;

  unsigned int volumeUid;
  std::vector< std::pair<unsigned int,Dart_handle> > volumeDartIndex;
  std::vector< char > volumeProperties;
};

#endif
