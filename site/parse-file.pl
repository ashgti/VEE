use MooseX::Declare;
use 5.12.1;

class ComponentSignal {
    has 'definition' => (is => 'ro', isa => 'Str');
    has 'id' => (is => 'rw', isa => 'Str');
}

class Serial extends ComponentSignal {
    method BUILD($ref) {
        $ref->{'definition'} =~ /Serial\s(\S+)/;
        $self->id($1);
        
        say "done with serial $1";
    }
    
    method parse($part) {
        say "adding a part";
    }
}

class Pin extends ComponentSignal {
    method BUILD($ref) {
        $ref->{'definition'} =~ /^Pin\s(\S+)(\s)?/;
        say "done with Pin $1 : $ref->{'definition'}";
        $self->id($1);
        if ($2) {
            /^Pin\s(\S+)(\s)/;
            say 'this one has more';
        }
    }
    
    method parse($line) {
        say "Found an output event";
    }
}

my $layout;
sub component_manager {
    my $component = shift;
    say $component;
    my $id = $component->id;
    say "My id: $id";
    say $id;
    if ($id && $layout->{$id}) {
        die 'pin already registered';
    }
    $layout->{$component->id} = $component;
}

class Det {
    has 'period' => (is => 'rw');
    has 'ratio' => (is => 'rw');
    
    method parse($id, $phase, $ratio) {
        if ($self->line =~ /det\s+(\d+),\s+(\d+(\.\d+)?),\s+(\d+(\.\d+)?)/) {
            $self->id($1);
            $self->period($2);
            $self->ratio($4);
        }
        else {
            die 'fail!';
        }
    }
    
    method time($time) {
        my $length =    $self->period
                     + ($self->period * $self->ratio);
        say "length of time: $length $time ", $time % $length;
        if ( ($time % $length) - $self->period ) {
            return 1;
        }
        else {
            return 0;
        }
    }
}

class Uni {
    method parse($line) {
        if ($self->line =~ /uni\s+(\d+),\s+(\d+)/) {
            say "Found a uniform distribution";
            $self->id($1);
        }
        else {
            die 'boom head shot';
        }
    }
}

class Exp {
    method parse($line) {
        if ($self->line =~ /exp\s+(\d+)/) {
            $self->id($1);
        }
        else {
            die 'and boom goes the dynamite';
        }
    }
}

open my $file, '<', 'sample-output.txt';
my $mode = 'header';
while (<$file>) {
    next unless $_;
    my $line = $_;
    $line =~ s/\n//;
    if ($mode eq 'header') {
        /^(\w+)/;
        my $s = $1;
        if (/---/) {
            $mode = 'body';
            next;
        }
        my $component;
        
        say "trying to make $s";
        $component = $s->new( definition => $line );
        component_manager($component) if $component;
    }
    elsif ($mode eq 'body') {
        /(\S+)\s+(.*)/;
        say $1;
        $layout->{$1}->parse($2);
    }
}
close $file;

print "\n\n";
say "Size is: " . keys %$layout;
for my $item (keys %$layout) {
    say $item;
}
