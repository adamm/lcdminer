#!/usr/bin/perl

use Device::SerialPort;
use IO::Socket;
use JSON;
use Data::Dumper;


my $port = Device::SerialPort->new("/dev/ttyUSB0");
$port->baudrate(115200);
$port->databits(8);
$port->parity("none");
$port->stopbits(1);
$port->handshake('none');
$port->write_settings();

my ($summary, $pools, $devs, $coin);

my ($out, $mhs, $pool, $a, $r, $u, $gpu_t, $gpu_f, $gpu_i, $block,
    @block_time);

while(1) {
    &get_data();
    &send_data();
    select(undef, undef, undef, 0.25);
}

sub get_data {
    $summary = &command("summary");
    $pools = &command("pools");
    $devs = &command("devs");
    $coin = &command("coin");

    $out = "";
    $mhs = $summary->[0]->{'MHS av'};
    $pool = $pools->[0]->{'Stratum URL'};
    $pool =~ s/^.*:\/\///;
    $pool =~ s/:.*$//;
    $a = $summary->[0]->{'Accepted'};
    $r = $summary->[0]->{'Rejected'};
    $u = $summary->[0]->{'Utility'};
    $gpu_t = $devs->[0]->{'Temperature'};
    $gpu_f = $devs->[0]->{'Fan Percent'};
    $gpu_i = $devs->[0]->{'Intensity'};
    $block = $coin->[0]->{'Current Block Hash'};
    @block_time = localtime($coin->[0]->{'Current Block Time'});
    $block_time[5] += 1900;
    $block_time[4] += 1;
}

my $rotate = 0;

sub send_data {
    $out .= &pad(sprintf("%s",$pool),20);
    $out .= &pad(sprintf("A:%d R:%d U:%.2f/m", $a, $r, $u) ,20);
    $out .= &pad(sprintf("%.2fMh/s",$mhs),10);
    $out .= &pad(sprintf("  %02d:%02d:%02d", $block_time[2], $block_time[1],
            $block_time[1]),10);
    # $out .= &pad(sprintf("B:%s", $block),10);
    $out .= &pad(sprintf("GPU0:%sC F:%s%% I:%d", $gpu_t, $gpu_f, $gpu_i),19);
    if ($rotate eq 0) {
        $out .= "|";
    }
    elsif ($rotate eq 1) {
        $out .= "/";
    }
    elsif ($rotate eq 2) {
        $out .= "-";
    }
    elsif ($rotate eq 3) {
        $out .= "`";
        $rotate = -1;
    }
    $rotate++;

    for(my $i = 0; $i < length($out); $i++) {
        my $c = substr($out, $i, 1);
        # print "out[$i]: $c\n";
        $port->write($c);
        select(undef,undef,undef,0.0025);
    }
    $port->write("\n");
    select(undef,undef,undef,0.0025);
}


sub pad {
    my $str = shift;
    my $len = shift;
    if (length($str) > $len) {
        return substr($str, 0, $len);
    }
    return $str . ' ' x ($len-length($str));
}


sub command {
    my $cmd = shift;
    my $param = shift;
    my $req = '{"command":"'.$cmd.'"';
    if ($param) {
        $req .= ',"parameter":"'.$param.'"';
    }
    $req .= '}';
    my $res = '';

    my $sock = IO::Socket::INET->new('192.168.1.128:4028') ||
            die "Error: $!\n";

    print $sock $req;;
    while (<$sock>) {
        $res .= $_;
    }
    $sock->close();

    my $obj = from_json($res);
    my $data = $obj->{uc $cmd};
    # print Dumper($data);
    return $data;
}
