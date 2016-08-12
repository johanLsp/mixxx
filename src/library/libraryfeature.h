// libraryfeature.h
// Created 8/17/2009 by RJ Ryan (rryan@mit.edu)

#ifndef LIBRARYFEATURE_H
#define LIBRARYFEATURE_H

#include <QAbstractItemModel>
#include <QFileDialog>
#include <QHash>
#include <QPointer>
#include <QString>
#include <QUrl>

#include "preferences/usersettings.h"
#include "track/track.h"

class Library;
class KeyboardEventFilter;
class TrackCollection;
class TrackModel;
class TreeItem;
class TreeItemModel;
class WBaseLibrary;
class WLibrary;
class WLibrarySidebar;
class WTrackTableView;

// This struct allows to save some data to allow interaction between
// the search bar and the library features
struct SavedSearchQuery {
    QString query;
    QString title;
    QSet<int> selectedItems;
    QString sortOrder;
    
    int vScrollBarPos;
    int sortColumn;
    bool sortAscendingOrder;
    
    // Used when saving and restoring from the DB
    int id;
};

// pure virtual (abstract) class to provide an interface for libraryfeatures
class LibraryFeature : public QObject {
    Q_OBJECT
  public:

    // The parent does not necessary be the Library
    LibraryFeature(UserSettingsPointer pConfig,
                   Library* pLibrary, TrackCollection *pTrackCollection, 
                   QObject* parent = nullptr);
    
    virtual ~LibraryFeature();

    virtual QVariant title() = 0;
    virtual QString getIconPath() = 0;
    QIcon getIcon();

    virtual bool dropAccept(QList<QUrl> /* urls */, 
                            QObject* /* pSource */) {
        return false;
    }
    virtual bool dropAcceptChild(const QModelIndex& /* index */,
                                 QList<QUrl> /* urls */, 
                                 QObject* /* pSource */) {
        return false;
    }
    virtual bool dragMoveAccept(QUrl /* url */) {
        return false;
    }
    virtual bool dragMoveAcceptChild(const QModelIndex& /* index */, 
                                     QUrl /* url */) {
        return false;
    }
    
    // Reimplement this to register custom views with the library widget
    // at the right pane.
    virtual QWidget* createPaneWidget(KeyboardEventFilter* pKeyboard, 
                                      int paneId);
    
    // Reimplement this to register custom views with the library widget,
    // at the sidebar expanded pane
    virtual QWidget* createSidebarWidget(KeyboardEventFilter* pKeyboard);
    
    virtual TreeItemModel* getChildModel() = 0;
    
    virtual void setFeatureFocus(int focus);
    virtual int getFeatureFocus();
    
    virtual void setFocusedPane(int paneId);
    
    virtual void saveQuery(SavedSearchQuery& query);
    virtual void restoreQuery(int index);
    virtual const QList<SavedSearchQuery> &getSavedQueries() const;

  public slots:
    // called when you single click on the root item
    virtual void activate() = 0;
    // called when you single click on a child item, e.g., a concrete playlist or crate
    virtual void activateChild(const QModelIndex&) {
    }
    // called when you right click on the root item
    virtual void onRightClick(const QPoint&) {
    }
    // called when you right click on a child item, e.g., a concrete playlist or crate
    virtual void onRightClickChild(const QPoint& /* globalPos */, 
                                   const QModelIndex& /* index */) {
    }
    // Only implement this, if using incremental or lazy childmodels, see BrowseFeature.
    // This method is executed whenever you **double** click child items
    virtual void onLazyChildExpandation(const QModelIndex&) {
    }
    
    virtual void onSearch(const QString&) {
    }
    
  signals:
    
    void loadTrack(TrackPointer);
    void loadTrackToPlayer(TrackPointer pTrack, QString group, bool play = false);
    // emit this signal before you parse a large music collection, e.g., iTunes, Traktor.
    // The second arg indicates if the feature should be "selected" when loading starts
    void featureIsLoading(LibraryFeature*, bool selectFeature);
    // emit this signal if the foreign music collection has been imported/parsed.
    void featureLoadingFinished(LibraryFeature*);
    // emit this signal to select pFeature
    void featureSelect(LibraryFeature* pFeature, const QModelIndex& index);
    // emit this signal to enable/disable the cover art widget
    void enableCoverArtDisplay(bool);
    void trackSelected(TrackPointer);
    
  protected:
    inline QStringList getPlaylistFiles() { return getPlaylistFiles(QFileDialog::ExistingFiles); }
    inline QString getPlaylistFile() { return getPlaylistFiles(QFileDialog::ExistingFile).first(); }
    
    // Creates a table widget with no model
    virtual WTrackTableView* createTableWidget(KeyboardEventFilter* pKeyboard, 
                                               int paneId);
    
    // Creates a WLibrarySidebar widget with the getChildModel() function as
    // model
    WLibrarySidebar* createLibrarySidebarWidget(KeyboardEventFilter* pKeyboard);
    
    // Override this function to create a custom inner widget for the sidebar,
    // the default widget is a WLibrarySidebar widget
    virtual QWidget* createInnerSidebarWidget(KeyboardEventFilter* pKeyboard);
    
    void showTrackModel(QAbstractItemModel *model);
    void switchToFeature();
    void restoreSearch(const QString& search);
    void showBreadCrumb(TreeItem* pTree);
    void showBreadCrumb(const QString& text, const QIcon &icon);
    void showBreadCrumb();
    
    WTrackTableView* getFocusedTable();
    
    UserSettingsPointer m_pConfig;
    Library* m_pLibrary;
    TrackCollection* m_pTrackCollection;
    
    int m_featureFocus;
    int m_focusedPane;
    
    QList<SavedSearchQuery> m_savedQueries;
    
  private: 
    QStringList getPlaylistFiles(QFileDialog::FileMode mode);
    QHash<int, QPointer<WTrackTableView> > m_trackTables;
};

#endif /* LIBRARYFEATURE_H */
