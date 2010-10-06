# when we get a INT signal, set the exit flag
$SIG{INT} = "shutdown";
$SIG{TERM} = "killmenow";

# load required modules
use strict;

use POSIX qw(setsid);
use Carp;
use POSIX;
use LWP::Simple;
use Date::Manip;
# use Proc::Daemon;
use Proc::PID::File;
# use Proc::ProcessTable;
use ConfigReader::Simple;

# use vars qw( $VERSION @ISA @EXPORT_OK );
# use Exporter;
# @ISA = qw( Exporter );
# $VERSION = "0.03";
# @EXPORT_OK = qw( Fork OpenMax );

my $pid;
our $version = '1.0';
our $DATE = 'date +"%b %d %T"';

# set variables
my $config = ConfigReader::Simple->new("radarwatch.conf");
my $updateFreq = 60*10;
my $logFile = "log/radarwatchd.log";
my $errorFile = "log/radarwatchd.err";
my $pidFile = "log/radarwatchd.pid";

# Assume we're using new type, old config is now deprecated
# my $configType = $config->get("ConfigType") || die "Unable to read ConfigType from config";

my $stationList = $config->get("StationList")  || die "Unable to read StationList from config";;
my $radarTypeList = $config->get("RadarTypeList")  || die "Unable to read RadarTypeList from config";;
my $downloadScript = $config->get("DownloadScript") || die "Unable to read DownloadScript from config";
my $downloadParameters = $config->get("DownloadParameters") || die "Unable to read DownloadParameters from config";
$updateFreq = $config->get("UpdateInterval") if ($config->get("UpdateInterval"));
$logFile = $config->get("DaemonLog") if ($config->get("DaemonLog"));
$errorFile = $config->get("ErrorLog") if ($config->get("ErrorLog"));
$pidFile = $config->get("PidFile") if ($config->get("PidFile"));


my $daemonName = $0;
my $command = $ARGV[0];


if ($command eq "start")
{
    start_daemon();
}
elsif ($command eq "stop")
{
    kill_daemon();
}
# elsif ($command eq "verifystart")
# {
#     check_daemon_start();
# }
# elsif ($command eq "restart")
# {
#     kill_daemon();
#     start_daemon();
# }
# elsif ($command eq "status")
# {
#     daemon_status();
# }
else {
    die "unknown command $command\n";
}
exit;


sub log_entry
{
	my ($handle, $entry) = @_;

	my $date = `$DATE`;
	chomp($date);
	print $handle $date . " radarwatchd: " . $entry . "\n";
}

##---------------------------------------------------------------------------##
##	Fork(): Try to fork if at all possible.  Function will croak
##	if unable to fork.
##
sub Fork {
    my($pid);
    FORK: {
	if (defined($pid = fork)) {
	    return; # $pid;
	} elsif ($! =~ /No more process/) {
	    sleep 5;
	    redo FORK;
	} else {
	    croak "Can't fork: $!";
	}
    }
}

##---------------------------------------------------------------------------##
##	OpenMax(): Return the maximum number of possible file descriptors.
##	If sysconf() does not give us value, we punt with our own value.
##
sub OpenMax {
    my $openmax = POSIX::sysconf( &POSIX::_SC_OPEN_MAX );
    (!defined($openmax) || $openmax < 0) ? 64 : $openmax;
}

##---------------------------------------------------------------------------##
##	Init(): Become a daemon.
##
sub Init {
    my $oldmode = shift || 0;
    my($pid, $sess_id, $i);

    # Fork and return parent
    if ($pid = Fork) { return $pid; }

    ## Detach ourselves from the terminal
    croak "Cannot detach from controlling terminal"
	unless $sess_id = POSIX::setsid();

    ## Prevent possibility of acquiring a controling terminal
    if (!$oldmode) {
		$SIG{'HUP'} = 'IGNORE';
		if ($pid = Fork) { exit 0; }
    }

    ## Change working directory
#     chdir "/";

    ## Clear file creation mask
    umask 0;

    ## Close open file descriptors
    foreach $i (0 .. OpenMax) { POSIX::close($i); }

	## Reopen stderr, stdout, stdin to /dev/null
    open(STDIN,  "+>/dev/null");
    open(STDOUT, "+>&STDIN");
    open(STDERR, "+>&STDIN");

    $oldmode ? $sess_id : 0;
}
*init = \&Init;

sub shutdown { $::exit = 1 }
sub killmenow {
	# Close all open filehandles and terminate immeadiately
	$::exit = 1
}

sub kill_daemon
{
 	$pid = Proc::PID::File->running(name => "radarwatchd", dir => "/tmp");
	unless ($pid) {
		die("$daemonName daemon not running!\n");
	}

	kill(2, $pid);
	print "stopping daemon...";

	# Use Proc::ProcessTable here to verify daemon is killed
	print "killed\n";

	exit;
}

sub start_daemon
{
	print "starting daemon...\n";

#     # Fork and return parent
#     if ($pid = Fork) {
# 		exit;
# # 		return $pid;
# 	}

#  	my $existingPid = Proc::PID::File->running(name => "radarwatchd", dir => "/tmp");
# 	if ($existingPid) {
# 		print "failed\ndaemon PID $existingPid already running\n";
# 		exit
# 	}

# 	Proc::Daemon::Init();
 	exit if (Init());
# 	if ($pid = Init())
# 	{
# # 		sleep 1;
#
# 		my $t = new Proc::ProcessTable;
# 		foreach my $p (@{$t->table}) {
# # 			print $p>fname(  );
# 			if ($p->cmndline() =~ /.*radarwatchd.*/i && $p->pid != $$) {
# # 				print "\nfound process: " . $p->pid . " " . $p->cmndline() . "\n";
# 				print "aborting\ndaemon PID " . $p->pid . " already running\n";
# 				exit;
# 			}
# 		}
#
# 		print "started with PID $pid\n";
# 		exit;
# 	}

	# Abort if daemon is alredy running/PID file exists
	if (Proc::PID::File->running(name => "radarwatchd", dir => "/tmp")) {
		die("$daemonName already running!\n");
	}

# 	## Reopen stderr, stdout, stdin to /dev/null
#     open(STDIN,  "+>/dev/null");
#     open(STDOUT, "+>&STDIN");
#     open(STDERR, "+>&STDIN");

	# Open logfile and set to unbuffered mode
	open(LOG, ">>$logFile");
	binmode(LOG, ":unix");
	my $LOG = *LOG;
	log_entry($LOG, "daemon v$version starting up");

	my $args = "$downloadScript \"$stationList\" \"$radarTypeList\" $downloadParameters";

    # our infinite loop
    while(1)
    {
		log_entry($LOG, "spawning '$args'");
 		system("bash $args >/dev/null");
# 		print "bash $args >/dev/null\n";
		log_entry($LOG, "sleeping for $updateFreq");
        sleep($updateFreq);
		if ($::exit) {
			log_entry($LOG, "shutting down");
			close(LOG);
			exit;
		}
    }
}

# sub kill_daemon
# {
#     my $daemon;
#
#     open PID, "<$pidFile" or die "Unable to open pidFile $pidFile\n";
#     my $pid = <PID>;
#
#     print "Searching for daemon PID $pid...";
#     system("ps -p $pid >/dev/null");
#
#     if ($? == -1) {
#         die "failed to execute: $!\n";
#     }
#     elsif ($? & 127) {
#         printf "child died with signal %d, %s coredump\n",
# 	    ($? & 127),  ($? & 128) ? 'with' : 'without';
#     }
#
#     $daemon = $? >> 8;
#     if ($daemon == 0)
#     {
#         system("kill $pid >/dev/null");
#         print ".";
#         system("ps -p $pid >/dev/null");
#         $daemon = $? >> 8;
#         print ".";
#         if ($daemon == 0)
#         {
#             print "unable to kill, exiting\n";
#             exit 1;
#         }
#         elsif ($daemon == 1)
#         {
#             print "killed\n";
#             system("chmod a+w $pidFile && rm $pidFile >/dev/null");
#             if ($? == -1) {
#                 print "failed to remove pidFile $pidFile\n";
#             }
#             open LOG, ">>$logFile" or warn "Unable to open log file $logFile\n";
#             print LOG "\nstopping daemon on " . UnixDate("today", "%l") . " ...\n";
#             close LOG;
#             #exit 0;
#         }
#     }
#     elsif ($daemon == 1) {
#         die "unable to find, aborting\n";
#     }
#     else {
#         die "\nunknown return value $daemon, aborting\n";
#     }
# }
#
# sub daemon_status
# {
#     my $daemon;
#
#     open PID, "<$pidFile" or die "daemon stopped\n";
#     my $pid = <PID>;
#
#     system("ps -p $pid >/dev/null");
#
#     if ($? == -1) {
#         die "failed to execute: $!\n";
#     }
#     elsif ($? & 127) {
#         printf "child died with signal %d, %s coredump\n",
# 	    ($? & 127),  ($? & 128) ? 'with' : 'without';
#     }
#     elsif ($? >> 8 == 0)
#     {
#         print "pid $pid running...\n";
#         exit 0;
#     }
#     elsif ($? >> 8 == 1)
#     {
#         print "daemon stopped, stale pid file ($pidFile) found (pid: $pid), ";
# # 		system("chmod o+w $pidFile");
# #         system("rm $pidFile >/dev/null");
# #         if ($? == -1) {
# #             print "failed to remove file $pidFile\n";
# #         }
# #         elsif ($? >> 8 == 0) {
# #             print "removed\n";
# #         }
#         exit;
#     }
#     else {
#         die "\nunknown return value $daemon, aborting\n";
#     }
# }
#
# sub daemonize {
#     #chdir '/'                 or die "Can't chdir to /: $!";
#     defined(my $pid = fork)   or die "Can't fork: $!";
#     if ($pid)
#     {
#         open PID, ">$pidFile" or warn "Unable to open pidFile $pidFile to write PID $pid\n";
#         print PID $pid or warn "Unable to write PID ($pid) to pidFile $pidFile\n";
#         close PID or warn "Unable to close pidFile $pidFile\n";
#         print "launching radarwatch daemon (pid = $pid)\n";
# 		system("chmod a-w $pidFile");
#         exit;
#     }
#     open STDIN, '/dev/null'   or die "Can't read /dev/null: $!";
#     open STDOUT, ">>$logFile" or die "Can't write to $logFile: $!";
#     open STDERR, ">>$errorFile" or die "Can't write to $errorFile: $!";
#     setsid                    or die "Can't start a new session: $!";
#     umask 0;
# }
