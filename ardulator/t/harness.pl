use Test::Harness;

my $harness
eval { require TAP::Harness };
if ($@) {
    Test::Harness::runtests(@tests);
    exit;
}
else {
    $harness = TAP::Harness->new({
        verbosity => $ENV{HARNESS_VERBOSE},
        merge     => 0,
        jobs      => $ENV{TEST_JOBS} || 1,
        directives => 1,
    });
}
my $results = $harness->runtests(@tests);

exit ( $results->all_passed() ? 0 : 1 );

