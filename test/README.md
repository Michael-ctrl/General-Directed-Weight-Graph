Rationale
=========

Overall
-------
For the testing process, I've broken the test files into their functionality going from top to bottom of the specification. During the testing of each function, basic tests were written up first before more indepth tests were added to test for any edge cases and tests were broken into specific cases. Graph assessors were used throughout most of the tests as it provided a good solution in testing the graph with an expected output. All undefined behaviour is ignored and not tested.
Tests were created to ensure the function specification was met.

## Constructors Test
It was important to test that all my constructors worked before moving onto the modifiers, iterators,  etc. This included initialising the graph with different node and edge types and to initialise the graph through iterators to different objects.

## Modifiers
With the graph constructors working, it was important to test deeper into the data structure of the graph. This meant testing the modifiers such as insert node and edges. Checking copy/move constructors to 'copy' the data over needed to be completed too to ensure the full functionality of those respective constructors. E.g. merge_replace_node should not have duplicate edges.

## Assessors
As the graphs can be constructed and modified, the next step was to check the assessor functions to ensure that each functions functionality was working correctly. The tests were written with the assumptions that constructors and modifiers were working and the output of the assessors were compared to an expected output.

## Iterator
Iteratators were tested to ensure that it traverses the graph correctly following the spec. By manually comparing the value of the iterator of the graph to an expected value, it checks that the iterator functions correctly.
Function Correctly - Iterates through the sorted set of edges and nodes.

## Comparisons
The operator '==' was used throughout testing of other functions to check that two graphs were equal. No individual test file was created for this as it would be slightly redundant.

## Extractor
Tested that outputs of the same graph are equal before testing against expected outputs of a complex graph was completed.