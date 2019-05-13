/* Create a GraphQL Query by using the graphql_ppx */
module GetPokemons = [%graphql
  {|
  query {
      pokemons(offset: 0) {
          id
          name
          sprites{
            default{
              front
            }
          }
      }
  }
|}
];

module GetPokemonsQuery = ReasonApollo.CreateQuery(GetPokemons);



[@react.component]
let make = () => {
  let getPokemonsQuery = GetPokemons.make();
      <GetPokemonsQuery>
        ...{
             ({result}) =>
               switch (result) {
               | Loading => <div> {ReasonReact.string("Loading")} </div>
               | Error(error) =>
                 <div> {ReasonReact.string(error##message)} </div>
               | Data(response) =>
                 <ul> {
                  /* Handles a deeply nested optional response */
                  response##pokemons
                  -> Belt.Array.mapWithIndex((i, pokemon) => {
                    <li key={string_of_int(i)}>
                      <img src={pokemon##sprites##default##front} />
                      <span>{ React.string(pokemon##name) }</span>
                    </li>
                  })
                  -> React.array
                 } </ul>
               }
           }
      </GetPokemonsQuery>;
}