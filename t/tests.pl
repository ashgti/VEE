use warnings;
use strict;
use Moose;

{
    package SignalMngr;
    use Moose;

    has 'signal_states' => (
        traits  => [qw( Hash )],
        isa     => 'HashRef',
        builder => '_build_mapping',
        handles => {
            mapping        => 'accessor',
            has_mapping    => 'exists',
            mappings       => 'keys',
            total_mappings => 'count',
            delete_mapping => 'delete',
        },
    );

    before 'mapping' => sub {
        my $self = shift;
        if (scalar @_ == 1) {
            my $key = shift;
            confess "The field $key does not exist"
                unless $self->has_mapping( $key );
        }
    };

    sub _build_mapping {
        return {};
    }
}

{
    package Arduino;
    use Moose;

    has 'signals' => (
        is => "rw",
        isa => "SignalMngr",
    );

    sub addSignal {
        my ($self, $key, $val) = @_;
        my $signals = $self->signals();
        $signals->mapping($key, $val);
    }

    sub dispatchSignal {
        my ($self, $key) = @_;
        my $signals = $self->signals();
        return $signals->mapping($key);
    }
}

my $a = Arduino->new(signals => SignalMngr->new());

$a->addSignal("a", 5);
$a->addSignal("b", 10);

my $time = 0;
$time += $a->dispatchSignal("b");
$time += $a->dispatchSignal("a");
print 'Yo! ' . $time;