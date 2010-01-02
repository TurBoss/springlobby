#ifndef SPRINGLOBBY_HEADERGUARD_TORRENTWRAPPER_H
#define SPRINGLOBBY_HEADERGUARD_TORRENTWRAPPER_H

#ifndef NO_TORRENT_SYSTEM

//#ifdef _MSC_VER
/// MSVC can not compile std::pair used in bimap with forward decl only.
/// GCC cant compile TorrentTable::Row either.
#include <libtorrent/torrent_handle.hpp>
//#endif

#include <wx/arrstr.h>
#include <wx/event.h>

#include <map>
#include <vector>
#include <queue>

#include "iunitsync.h"
#include "thread.h"

/*
namespace libtorrent{ class session; };
namespace libtorrent { struct torrent_handle; };
*/
class TorrentWrapper;
class PlasmaInterface;
class PlasmaResourceInfo;

namespace P2P {
enum FileStatus
{
    /// Dont change values. Bit arithmetics is used in TorrentTable::Row
    not_stored=0, /// file is not on disk and not downloading
    queued=1, /// file is not on disk and queued for download
    leeching=2,/// file is being downloaded
    stored=128,/// file is on disk
    seeding=129/// file is on disk and being seeded
};
}

struct TorrentInfos
{
    float numcopies;
    wxString name;
    unsigned int downloaded;
    P2P::FileStatus downloadstatus;
    float progress;
    float inspeed;
    unsigned int filesize;

    int eta;

	//default constructor
	TorrentInfos() : numcopies(-1.f), downloaded(0), downloadstatus(P2P::not_stored), progress(0.f), inspeed(0.f), filesize(0), eta(0) {}
};


class TorrentMaintenanceThread : public Thread
{
	public:
		TorrentMaintenanceThread( TorrentWrapper* parent );
		void Init();
		void Stop();
		void* Entry();

	protected:
		bool TestDestroy();

		bool m_stop_thread;
		TorrentWrapper& m_parent;
};


class TorrentWrapper
{
public:

    TorrentWrapper();
    ~TorrentWrapper();

    enum DownloadRequestStatus
    {
        success,
        not_connected,
        paused_ingame,
        duplicate_request,
        file_not_found,
        torrent_join_failed,
        scheduled_in_cue,
        missing_in_table,
        remote_file_dl_failed,
        corrupt_torrent_file,
        no_seeds_found
    };

    /// gui interface
    bool IsFileInSystem( const wxString& name );
    P2P::FileStatus GetTorrentStatusByName(const wxString& name );
    bool RemoveTorrentByName( const wxString& name );
    int GetTorrentSystemStatus();

    ///HashToTorrentData& GetSystemFileList();

    /// lobby interface
    void SetIngameStatus( bool status );

	//! will add name to a queue that's processed from the maint. thread, to avoid GUI blocking (maybe)
    void RequestFileByName( const wxString& name );

    //!remove all torrents that have seed status
    void ClearFinishedTorrents();

    void UpdateSettings();
    std::map<wxString,TorrentInfos> CollectGuiInfos();

	//! threaded maintenance tasks
    void JoinRequestedTorrents();
    void RemoveUnneededTorrents();
	void HandleCompleted();
    void TryToJoinQueuedTorrents();
    void SearchAndGetQueuedDependencies();
    void ResumeFromList();

private:
    DownloadRequestStatus _RequestFileByName( const wxString& name );
	void DisplayError( const wxString& resourcename, DownloadRequestStatus );

    typedef std::vector<libtorrent::torrent_handle>
        TorrenthandleVector;
    //! internal
    typedef std::map<PlasmaResourceInfo,libtorrent::torrent_handle>
        TorrenthandleInfoMap;
    TorrenthandleInfoMap m_handleInfo_map;

    DownloadRequestStatus AddTorrent( const PlasmaResourceInfo& info );

	typedef std::queue<wxString>
		JoinQueue;
	JoinQueue m_join_queue;

    wxString m_buffer;

    bool ingame;
    unsigned int m_timer_count;

    wxArrayString m_tracker_urls;

    TorrentMaintenanceThread m_maintenance_thread;

    libtorrent::session* m_torr;

    bool m_started;

    wxMutex m_info_map_mutex;
};


TorrentWrapper& torrent();

#endif

#endif // SPRINGLOBBY_HEADERGUARD_TORRENTWRAPPER_H

/**
    This file is part of SpringLobby,
    Copyright (C) 2007-09

    springsettings is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as published by
    the Free Software Foundation.

    springsettings is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SpringLobby.  If not, see <http://www.gnu.org/licenses/>.
**/

