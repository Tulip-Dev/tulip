/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

///@cond DOXYGEN_HIDDEN

#ifndef GRAPHNEEDSSAVINGOBSERVER_H
#define GRAPHNEEDSSAVINGOBSERVER_H

#include <tulip/Observable.h>

#include <QObject>

class QMainWindow;

namespace tlp {

class Graph;

/**
 * @see Observable
 *
 * @brief The GraphNeedsSavingObserver class will observe a graph and tells if
 * it has been modified.
 *
 * @li The constructor. Observe the graph given in parameter for modification
 * @li needsSaving. Returns true is the graph has been modified
 * @li saved. The graph has been saved, and the status of the class must be
 * reset. needsSaving will return false if called after saved().
 * @li savingNeeded. Signal send when the status of the graph evolves.
 *
 */
class TLP_QT_SCOPE GraphNeedsSavingObserver : public QObject, Observable {

  Q_OBJECT

  bool _needsSaving;
  Graph *_graph;
  QMainWindow *_mainWindow;

  void addObserver();
  void removeObservers();

public:
  /**
   * @brief GraphNeedsSavingObserver Class constructor
   * @param graph The graph which needs to be observed for modifications
   * @param mainWindow The Qt QMainWindow object behin the perspective
   */
  GraphNeedsSavingObserver(Graph *graph, QMainWindow *mainWindow = nullptr);

  /**
   * @brief saved If the graph has been saved, one has to call this method to
   * reset the status of the graph (it does not need to be saved). to indicate
   * that the graph does not need to be saved until a new modification.
   */
  void saved();

  /**
   * @brief needsSaving Indicates if the graph has been modified, and thus needs
   * to be saved.
   *
   * @return true if the graph needs to be saved, false otherwise.
   */
  bool needsSaving() const;

  /**
   *
   * @brief forceToSave Even if there is no modification on the graph, this
   * method can be used to force to save the graph.
   */
  void forceToSave();

protected:
  /**
   * @see Listener
   * @see Observer
   * @see Observable
   * @see Observable::treatEvents(const std::vector<Event>&)
   *
   * @brief treatEvents This function is called when events are sent to
   * Observers, and Observers only.
   *
   * @param events The events that happened since the last unHoldObservers().
   */
  void treatEvents(const std::vector<Event> &) override;

signals:

  /**
   * @brief savingNeeded This signal is sent when the graph needs to be saved
   * (it has been modified).
   */

  void savingNeeded();
};
} // namespace tlp
#endif // GRAPHNEEDSSAVINGOBSERVER_H

///@endcond
