let getNonEmptyObj = jsObj => 
switch (jsObj |> Js.Nullable.to_opt) {
    | None => None
    | Some(data) => 
      switch (Js.Json.decodeObject(data)) {
      | None => None
      | Some(data) => 
        switch (Array.length(Js.Dict.keys(data))) {
        | 0 => None
        | _ => Some(Js.Json.object_(data))
        }
      };
    };