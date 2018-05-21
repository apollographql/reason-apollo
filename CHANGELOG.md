V 0.8.0
Breaking changes :
 - Delete the NoData variant in the result of the Query and Mutation components since there always has to be an Error or Data.
 - Mutation component: result variant Data tag only appeared when error was None and data Some. Now, only data has to be Some.
 - Mutation `Called` tag is deleted in result, so pattern matching is easier. 
Features:
  - Expand definition of ApolloError to be able to construct one

Fixes:
  - Mutation result pattern matching

