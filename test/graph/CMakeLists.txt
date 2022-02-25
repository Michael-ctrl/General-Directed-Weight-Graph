cxx_test(TARGET graph_test1 FILENAME "graph_test1.cpp")

   cxx_test(TARGET constructor_test FILENAME "constructor_test.cpp")

      cxx_test(TARGET modifier_test FILENAME "modifier_test.cpp")

         cxx_test(TARGET extractor_test FILENAME "extractor_test.cpp")

            cxx_test(TARGET iterator_test FILENAME "iterator_test.cpp")

               cxx_test(TARGET merge_replace_nodes_test FILENAME "merge_replace_nodes_test.cpp")

                  cxx_test(TARGET accessor_test FILENAME "accessor_test.cpp")