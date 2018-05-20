V 0.8.0
Breaking changes :
 - Delete the NoData variant in the result of the Query and Mutation components since there always has to be an Error or Data.
 - Mutation component: result variant Data tag only appeared when error was None and data Some. Now, only data has to be Some.
